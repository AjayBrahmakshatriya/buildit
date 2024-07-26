#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 76;
  builder::dyn_var<int> var1 = 58;
  builder::dyn_var<int> var2 = 90;
  builder::dyn_var<int> var3 = 4;
  builder::dyn_var<int> var4 = 41;
  while (var2 <= var4) {
    if (var3 < var1) {
      if (var3 == var1) {
        continue;
      } else {
        var3 = var0 + 33;
      }
    } 
    if (var0 <= var0) {
      while (var3 <= var1) {
        if (var0 != var0) {
        } 
      }
      if (var0 == var1) {
      } 
      if (var4 >= var0) {
        if (var2 == var4) {
          var4 = var0 + 77;
          var0 = var1 + 27;
        } else {
          break;
        }
      } 
    } else {
      var3 = var2 + 80;
      break;
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
