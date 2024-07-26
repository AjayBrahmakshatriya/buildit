#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 12;
  builder::dyn_var<int> var1 = 45;
  builder::dyn_var<int> var2 = 39;
  builder::dyn_var<int> var3 = 47;
  builder::dyn_var<int> var4 = 14;
  while (var2 < var4) {
    if (var4 > var0) {
      if (var0 > var1) {
        if (var3 >= var0) {
          continue;
        } 
        if (var1 <= var3) {
          continue;
        } 
        if (var1 < var1) {
          var0 = var1 + 49;
          var3 = var2 + 47;
        } else {
          var0 = var0 + 84;
        }
      } else {
        while (var3 <= var4) {
        }
        break;
      }
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
