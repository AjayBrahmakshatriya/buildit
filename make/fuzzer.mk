SEEDS=2844 328 32336 1083 30700 6785 15419 10904 2507 21194 14350 21133 22921 20690 14106 18958 10664 14326 5073 18842 13357 18599 2773 29745 8640 10623 15483 21186 27745 27850 7465 5379 17677 14022 594 20613 9193 22751 19803 10012 12756 18792 25667 12056 18999 7928 22047 20305 9784 25875 9740 16074 11223 4483 24737 14312 11832 28335 25673 13047 30089 19480 11355 14066 29364 16751 18683 14886 13928 11913 23331 1011 28780 3367 27583 24185 30820 30195 21355 2711 12321 31198 1879 15004 16289 28978 3988 15802 5661 27538 11397 11929 7256 14931 23129 31166 2399 16627 9700 29107

.PHONY: fuzz-gen

fuzz-gen: $(BUILD_DIR)/fuzzer
	mkdir -p $(TEST_DIR)/outputs/
	count=1;\
	for s in $(SEEDS); do \
		$(BUILD_DIR)/fuzzer $$s > $(TEST_DIR)/inputs/test$$count.cpp 2> $(TEST_DIR)/outputs/test$$count; \
		count=$$((count+1)); \
	done;


FUZZ_TEST_CASES=$(wildcard $(TEST_DIR)/inputs/*.cpp)
FUZZ_EXECUTABLES=$(subst $(TEST_DIR)/inputs,$(BUILD_DIR)/tests/exes,$(FUZZ_TEST_CASES:.cpp=))

$(BUILD_DIR)/tests/exes/%: $(TEST_DIR)/inputs/%.cpp $(INCLUDES) $(DEPS_LIST) $(LIBRARY)
	@mkdir -p $(@D)
	$(CXXLDV) $(CFLAGS_INTERNAL) $(CFLAGS) $< -o $@ $(INCLUDE_FLAGS) $(LINKER_FLAGS)


fuzz-tests: $(FUZZ_EXECUTABLES)

SORTED_FUZZERS=$(shell echo $(FUZZ_EXECUTABLES) | tr " " "\n" | sort -V | tr "\n" " ")
fuzz-run: SHELL:=/bin/bash
fuzz-run: fuzz-tests
	@ if [ "$(TEST)" == "" ]; then \
		@ total=0;\
		success=0;\
		progress="";\
		fail="";\
		for sample in $(SORTED_FUZZERS); do \
			sample_name=$$(basename $$sample); \
			((total=total+1)); \
			if cmp -s $(TEST_DIR)/outputs/$$sample_name <($$sample); then \
				echo -e "\e[32m"$$sample_name: "OK\e[39m"; \
				((success=success+1)); \
				progress=$$progress"\e[32m#\e[39m"; \
			else \
				echo -e "\e[31m"$$sample_name: "FAIL\e[39m"; \
				fail=$$fail"\e[31mX\e[39m"; \
			fi; \
		done; \
		echo -e "["$$progress$$fail"] "$$success/$$total; \
		if [[ $$total == $$success ]]; then \
			exit 0; \
		else \
			exit 1; \
		fi \
	else	\
		diff $(TEST_DIR)/outputs/$(TEST) <($(BUILD_DIR)/tests/exes/$(TEST)) || exit 1;	\
		echo $(TEST): OK; \
	fi
