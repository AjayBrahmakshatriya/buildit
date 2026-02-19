// Include the headers
#include "blocks/c_code_generator.h"
#include "builder/static_var.h"
#include "builder/dyn_var.h"
#include <iostream>

// Include the BuildIt types
using builder::dyn_var;
using builder::static_var;


struct instance {
    constexpr static const char* type_name = "instance";
    dyn_var<int> mem = builder::with_name("mem");
};


template <typename T>
dyn_var<T> select(const dyn_var<T> &cond, const dyn_var<T>& t, const dyn_var<T>& f) {
        dyn_var<T> res;
        if (cond) {
                res = t;
        } else {
                res = f;
        }
        return res;
}


static dyn_var<int> bar (builder::dyn_var<instance*> here) {
  dyn_var<double> delvbs, delvded;
  if (67 == here->mem || select<double>(delvbs, delvbs, delvded)) {
  }
  return 167;
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar");
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}



