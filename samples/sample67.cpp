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
                        if (!(bool)j) 
                            break;
                        x = 1;
                }
        }
        x = 4;



	for (dyn_var<int> i = 0; i < 10; i++) {
		x = 1;
		if (x) continue;
		x = 2;
		if (!x) break;
		if (x + 2) continue;
		x = 3;
		break;
	}

	dyn_var<int> i, j;
	while (i == 0) {
		while (j == 0) {
			x = x + 1;
		}
		x = x - 1;
	}

}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar");
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}


