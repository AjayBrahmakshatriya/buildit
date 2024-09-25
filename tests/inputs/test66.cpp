#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 16;
  builder::dyn_var<int> var1 = 35;
  builder::dyn_var<int> var2 = 66;
  builder::dyn_var<int> var3 = 99;
  builder::dyn_var<int> var4 = 74;
  var2 = var3 + 95;
  if (var3 <= var1) {
    var1 = var4 + 84;
    if (var0 > var1) {
      if (var2 > var0) {
        if (var4 < var0) {
          var0 = var3 + 3;
        } else {
          var0 = var3 + 14;
        }
        if (var0 != var1) {
          var2 = var4 + 87;
        } 
        var0 = var1 + 66;
      } 
    } else {
      while (var1 >= var2) {
      }
    }
    var1 = var1 + 63;
  } else {
    if (var3 <= var0) {
      var2 = var1 + 99;
      if (var4 != var1) {
        while (var0 >= var4) {
          var2 = var4 + 62;
          var3 = var1 + 51;
        }
        var4 = var2 + 82;
      } 
      while (var4 != var2) {
        if (var2 > var3) {
        } 
      }
    } 
    var4 = var1 + 10;
    if (var2 != var3) {
      var0 = var2 + 88;
      while (var4 < var0) {
        while (var0 == var1) {
          var1 = var0 + 93;
        }
      }
      var4 = var3 + 22;
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
