#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 50;
  builder::dyn_var<int> var1 = 41;
  builder::dyn_var<int> var2 = 40;
  builder::dyn_var<int> var3 = 71;
  builder::dyn_var<int> var4 = 63;
  if (var2 == var4) {
    var0 = var1 + 33;
  } else {
    var3 = var3 + 80;
    var4 = var2 + 24;
  }
  while (var0 >= var2) {
    var2 = var4 + 13;
  }
  if (var2 <= var3) {
    var0 = var0 + 89;
    var3 = var1 + 67;
  } else {
    var1 = var2 + 67;
    while (var1 != var2) {
      if (var1 >= var2) {
        if (var1 != var2) {
          var2 = var0 + 65;
        } 
        break;
      } else {
        if (var0 < var1) {
          if (var2 >= var4) {
            var3 = var2 + 76;
            break;
          } else {
            var0 = var4 + 45;
            break;
          }
          break;
        } else {
          while (var0 != var3) {
          }
          while (var2 == var3) {
          }
        }
      }
    }
    var2 = var1 + 59;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
