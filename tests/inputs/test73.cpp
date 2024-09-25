#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 72;
  builder::dyn_var<int> var1 = 34;
  builder::dyn_var<int> var2 = 67;
  builder::dyn_var<int> var3 = 89;
  builder::dyn_var<int> var4 = 80;
  var3 = var0 + 63;
  if (var1 >= var4) {
    if (var3 < var4) {
      var2 = var2 + 76;
      if (var3 <= var0) {
        if (var3 > var4) {
          var1 = var4 + 2;
        } else {
          var0 = var2 + 44;
          var2 = var0 + 20;
        }
        if (var2 > var1) {
          var4 = var1 + 93;
          var3 = var1 + 2;
        } 
      } 
      if (var2 >= var4) {
        while (var4 < var2) {
          var3 = var1 + 14;
        }
      } else {
        while (var3 == var2) {
        }
        var1 = var2 + 32;
      }
    } else {
      var3 = var0 + 90;
      if (var4 == var0) {
        var1 = var3 + 32;
        var2 = var2 + 69;
        while (var1 < var0) {
        }
      } 
    }
    if (var0 < var1) {
      var2 = var2 + 42;
    } 
    if (var1 <= var4) {
    } 
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
