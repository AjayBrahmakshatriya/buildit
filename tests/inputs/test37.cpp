#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 47;
  builder::dyn_var<int> var1 = 51;
  builder::dyn_var<int> var2 = 80;
  builder::dyn_var<int> var3 = 60;
  builder::dyn_var<int> var4 = 60;
  if (var1 == var4) {
    var2 = var0 + 99;
    if (var3 != var1) {
      if (var4 != var0) {
        var3 = var3 + 15;
      } else {
        if (var4 > var1) {
        } 
        while (var2 >= var1) {
          var0 = var0 + 76;
        }
      }
      if (var3 != var2) {
        if (var4 < var4) {
          var4 = var4 + 32;
          var3 = var0 + 87;
        } 
        if (var3 >= var1) {
          var3 = var3 + 43;
          var2 = var1 + 61;
          var2 = var1 + 23;
        } 
        var4 = var2 + 98;
      } else {
        var0 = var2 + 43;
        var4 = var0 + 46;
      }
    } else {
      var3 = var0 + 26;
    }
    while (var3 < var0) {
      if (var2 >= var3) {
        break;
      } 
    }
  } 
  while (var1 > var4) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
