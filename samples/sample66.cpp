#include "builder/builder_union.h"
#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"

using builder::dyn_var;
template <typename T>
using var = builder::builder_union<T>;
using arg = builder::placeholder;

dyn_var<int> res_r = builder::with_name("res_r");
dyn_var<int> x_r = builder::with_name("x_r");

static var<int> power(var<int> base, var<int> exponent) {
	var<int> res = 1, x = base;
	while (exponent > 0) {
		if (exponent % 2 == 1) {
			res = res * x;
		}
		x = x * x;
		exponent = exponent / 2;
	}
	return res;
}

int main(int argc, char* argv[]) {
	builder::builder_context ctx;
	ctx.run_rce = true;
	auto ast1 = ctx.extract_function_ast(power, "power1", 2, 15);
	auto ast2 = ctx.extract_function_ast(power, "power2", arg(), 15);
	auto ast3 = ctx.extract_function_ast(power, "power3", 2, arg());
	auto ast4 = ctx.extract_function_ast(power, "power4", arg(), arg());
	//block::c_code_generator::generate_code(ast1, std::cout, 0);
	//block::c_code_generator::generate_code(ast2, std::cout, 0);
	block::c_code_generator::generate_code(ast3, std::cout, 0);
	//block::c_code_generator::generate_code(ast4, std::cout, 0);
	return 0;
}
