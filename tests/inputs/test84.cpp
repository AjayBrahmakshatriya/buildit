#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 10;
  builder::dyn_var<int> var1 = 44;
  builder::dyn_var<int> var2 = 24;
  builder::dyn_var<int> var3 = 87;
  builder::dyn_var<int> var4 = 84;
  while (var1 < var3) {
    var2 = var3 + 15;
    if (var1 < var4) {
    } 
    while (var4 >= var1) {
      if (var0 <= var1) {
        if (var1 <= var3) {
        } 
      } else {
        break;
      }
    }
  }
  if (var1 >= var0) {
    if (var1 > var2) {
    } 
  } else {
    if (var0 <= var1) {
      if (var4 == var0) {
        if (var4 == var2) {
          var2 = var4 + 5;
        } else {
          var1 = var0 + 91;
          var0 = var4 + 59;
          var3 = var3 + 33;
        }
      } 
      var0 = var1 + 87;
      while (var3 >= var4) {
        var3 = var2 + 43;
        var0 = var2 + 83;
      }
    } 
  }
  if (var2 != var1) {
    var3 = var4 + 51;
    while (var4 < var3) {
    }
    if (var4 != var0) {
      var3 = var1 + 83;
    } 
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
