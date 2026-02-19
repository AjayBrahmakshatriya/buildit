// Include the headers
#include "blocks/c_code_generator.h"
#include "builder/static_var.h"
#include "builder/dyn_var.h"
#include <iostream>

// Include the BuildIt types
using builder::dyn_var;
using builder::static_var;


dyn_var<int> select (dyn_var<bool> cond, dyn_var<int> t, dyn_var<int> f);
dyn_var<int> select (dyn_var<bool> cond, dyn_var<int> t, dyn_var<int> f) {
    return cond? t : f;
}

static void bar(dyn_var<int> a, dyn_var<int> b, dyn_var<int> c) {
     // Insert code to stage here
     
     
    dyn_var<int> x = a? b + 1: c + 1;
    
    dyn_var<int> y = x;
    
    
    if (y || (dyn_var<int>)(a ? b + 1: c + 1)) {
        x = y;
    }
     
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar");
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}


