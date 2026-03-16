// Include the headers
#include "blocks/c_code_generator.h"
#include "builder/static_var.h"
#include "builder/dyn_var.h"
#include <iostream>

// Include the BuildIt types
using builder::dyn_var;
using builder::static_var;

static void bar(void) {
     // Insert code to stage here
        dyn_var<int> x = 0;
        for (dyn_var<int> i = 0; i < 10; i++) {
                for (dyn_var<int> j = 0; j < 10; j++) {
                        if (j) break;
                        x = 1;
                }
        }
        x = 4;
}

int main(int argc, char* argv[]) {
        builder::builder_context context;
        auto ast = context.extract_function_ast(bar, "bar");
        block::c_code_generator::generate_code(ast, std::cout, 0);
        return 0;
}
