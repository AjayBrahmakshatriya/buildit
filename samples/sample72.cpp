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
	builder_union<int> x = 1;
	builder_union<int> w = x;
	builder_union<int> y = 3;
	builder_union<int> z = 5;
	builder_union<int> a = 7;
	builder_union<int> b = 7;
	
	b = in;
	a = in;	
	z = in;	
	y = in;
	x = in;

}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar");
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}
