#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 89;
  builder::dyn_var<int> var1 = 7;
  builder::dyn_var<int> var2 = 60;
  builder::dyn_var<int> var3 = 78;
  builder::dyn_var<int> var4 = 31;
  var0 = var4 + 96;
  if (var0 >= var3) {
    while (var1 != var1) {
      if (var4 > var3) {
        if (!(var0 >= var4)) {
          var4 = var3 + 67;
          var4 = var3 + 38;
          var0 = var1 + 97;
        } 
      } 
      while (var2 < var2) {
        var0 = var4 + 43;
        var0 = var2 + 2;
        if (var4 < var2) {
        } 
      }
      if (var4 <= var0) {
      } 
    }
    if (var1 != var3) {
      var3 = var0 + 37;
      var2 = var0 + 10;
    } else {
      var1 = var2 + 29;
      var1 = var4 + 88;
      if (var1 < var0) {
        if (var0 < var3) {
          var2 = var4 + 14;
          var2 = var3 + 19;
        } 
      } else {
        var4 = var0 + 21;
        while (var3 <= var2) {
        }
      }
    }
  } 
  while (var3 > var3) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
