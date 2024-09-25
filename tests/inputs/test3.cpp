#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 70;
  builder::dyn_var<int> var1 = 84;
  builder::dyn_var<int> var2 = 59;
  builder::dyn_var<int> var3 = 42;
  builder::dyn_var<int> var4 = 22;
  if (var2 > var3) {
    while (var3 != var1) {
      if (var1 >= var0) {
        var2 = var3 + 28;
        if (var2 != var4) {
        } 
      } else {
        break;
      }
    }
    if (var2 >= var3) {
      var3 = var4 + 14;
      var0 = var2 + 17;
    } else {
      var2 = var2 + 85;
    }
  } 
  var0 = var1 + 96;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
