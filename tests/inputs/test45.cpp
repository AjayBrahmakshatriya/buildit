#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 52;
  builder::dyn_var<int> var1 = 56;
  builder::dyn_var<int> var2 = 81;
  builder::dyn_var<int> var3 = 26;
  builder::dyn_var<int> var4 = 36;
  if (var1 < var3) {
    if (var3 > var1) {
      if (var0 < var1) {
      } 
    } 
  } 
  if (var1 >= var3) {
    if (var2 == var0) {
      var4 = var0 + 63;
      var2 = var3 + 87;
      while (var4 != var4) {
      }
    } else {
      if (var0 != var1) {
        if (var0 >= var2) {
          var1 = var1 + 38;
        } 
        if (var3 != var1) {
          var3 = var0 + 1;
        } 
        var1 = var3 + 6;
      } else {
        if (var4 <= var3) {
          var2 = var0 + 23;
          var4 = var1 + 1;
        } 
        while (var3 >= var2) {
        }
      }
      while (var3 == var2) {
        while (var2 == var0) {
          var3 = var3 + 90;
        }
      }
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
