#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 46;
  builder::dyn_var<int> var1 = 98;
  builder::dyn_var<int> var2 = 84;
  builder::dyn_var<int> var3 = 88;
  builder::dyn_var<int> var4 = 10;
  var1 = var4 + 83;
  if (var3 <= var2) {
    if (var1 > var2) {
    } 
  } 
  if (var0 != var0) {
    var1 = var0 + 46;
    while (var0 < var4) {
      var0 = var4 + 55;
      while (var2 >= var3) {
        while (var0 <= var0) {
        }
      }
    }
  } else {
    if (var2 >= var1) {
      while (var1 != var0) {
        var3 = var2 + 20;
      }
    } else {
      if (var3 >= var4) {
        if (var2 > var4) {
          var2 = var0 + 99;
          var4 = var1 + 91;
        } else {
          var1 = var0 + 33;
          var0 = var4 + 1;
        }
        var1 = var3 + 44;
        if (var2 >= var1) {
          var0 = var4 + 99;
        } 
      } else {
        var2 = var2 + 8;
        var1 = var0 + 56;
      }
    }
    while (var2 > var2) {
      if (var4 >= var1) {
        while (var0 <= var3) {
          var0 = var1 + 31;
        }
      } 
      var4 = var2 + 85;
    }
    var2 = var3 + 67;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
