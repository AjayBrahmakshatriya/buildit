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

static void bar(dyn_var<int> in) {
	builder_union<int> i(0);
	for (; i < 3; i = i + 1) {
		builder_union<int> x = i;
		builder_union<int> w = x;
		builder_union<int> y = i + 10;
		if (i == 2) {
			y = in;
			x = in;
		}
	}
	i = in;
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar");
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}
