#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 99;
  builder::dyn_var<int> var1 = 9;
  builder::dyn_var<int> var2 = 59;
  builder::dyn_var<int> var3 = 9;
  builder::dyn_var<int> var4 = 58;
  if (var3 > var0) {
    var4 = var0 + 87;
    while (var1 == var3) {
      while (var4 <= var3) {
        while (var3 < var2) {
        }
        var0 = var4 + 57;
      }
    }
  } else {
    var2 = var2 + 86;
    while (var1 <= var0) {
      if (var4 == var3) {
        while (var2 != var4) {
        }
      } 
      while (var0 != var4) {
      }
    }
    var2 = var0 + 87;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
