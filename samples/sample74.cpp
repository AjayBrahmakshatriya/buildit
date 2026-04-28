// Include the headers
#include "blocks/c_code_generator.h"
#include "builder/static_var.h"
#include "builder/dyn_var.h"
#include "builder/builder_union.h"
#include <iostream>

// Include the BuildIt types
using builder::dyn_var;
using builder::static_var;
using builder::builder_union;

static void bar(dyn_var<int> in, int loop) {
	builder_union<int> i = 0;
	for (; i < loop; i = i + 1) {
		builder_union<int> l1 = -1;
		builder_union<int> l2 = -1;
		dyn_var<int*> buff;
		for (l1 = 0; l1 < 64; l1 = l1 + 1) {
			for (l2 = 0; l2 < 64; l2 = l2 + 1) {
				builder_union<int> index = l1 * 64 + l2;
				buff[index] = l1;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar", 1);
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}
