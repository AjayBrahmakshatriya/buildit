// Include the headers
#include "blocks/c_code_generator.h"
#include "builder/static_var.h"
#include "builder/dyn_var.h"
#include "blocks/rce.h"
#include <iostream>

// Include the BuildIt types
using builder::dyn_var;
using builder::static_var;

static void bar(void) {
  builder::dyn_var<int> var0 = 57;
  builder::dyn_var<int> var1 = 45;
  builder::dyn_var<int> var2 = 25;
  builder::dyn_var<int> var3 = 28;
  builder::dyn_var<int> var4 = 85;
  if (var1 >= var2) {
    var1 = var1 + 90;
    if (var1 < var2) {
      var4 = var0 + 37;
    } else {
      if (var4 > var1) {
        var2 = var3 + 40;
      }
      while (var3 < var2) {
        if (var4 == var3) {
        } 
      }
    }
  }
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(bar, "bar");
	block::eliminate_redundant_vars(ast);
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}


