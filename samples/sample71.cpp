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

static void bar(void) {
     // Insert code to stage here
	for (dyn_var<int> i = 0; i < 64; i++) {
	for (dyn_var<int> j = 0; j < 64; j++) {
	for (builder_union<int> k = 0; k < 64; k = k + 1) {
	for (builder_union<int> l = 0; l < 64; l = l + 1) {
		i + j + k + l;
	}
	}
	}
	}

}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar");
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}


