#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 13;
  builder::dyn_var<int> var1 = 33;
  builder::dyn_var<int> var2 = 11;
  builder::dyn_var<int> var3 = 37;
  builder::dyn_var<int> var4 = 52;
  while (var0 > var0) {
    if (var1 > var3) {
      while (var0 < var3) {
      }
    } else {
      if (var2 > var1) {
        if (var0 <= var3) {
          break;
        } else {
          break;
        }
        break;
      } else {
        if (var3 == var4) {
          break;
        } 
      }
      while (var2 != var3) {
      }
      break;
    }
    while (var3 >= var3) {
      if (var1 < var4) {
        if (var0 < var0) {
          break;
        } else {
          var2 = var4 + 38;
          var1 = var3 + 49;
          continue;
        }
        var0 = var4 + 47;
      } 
      while (var3 == var3) {
      }
      var2 = var2 + 46;
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
