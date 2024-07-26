#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 38;
  builder::dyn_var<int> var1 = 99;
  builder::dyn_var<int> var2 = 39;
  builder::dyn_var<int> var3 = 13;
  builder::dyn_var<int> var4 = 45;
  if (var0 <= var4) {
    while (var1 < var1) {
      var4 = var2 + 67;
    }
  } else {
    while (var3 == var3) {
      var0 = var2 + 88;
    }
  }
  if (var0 <= var4) {
    if (var4 <= var0) {
      var4 = var2 + 35;
      var0 = var2 + 30;
      if (var1 < var0) {
        var2 = var4 + 91;
      } 
    } else {
      if (var4 > var4) {
      } 
    }
  } 
  if (var3 < var1) {
    if (var2 == var3) {
      if (var4 <= var0) {
        var4 = var1 + 16;
        if (var1 >= var2) {
        } 
        if (var1 <= var0) {
          var4 = var0 + 76;
          var4 = var1 + 63;
        } 
      } 
    } 
  } else {
    while (var3 != var1) {
      var1 = var2 + 58;
    }
    var3 = var2 + 85;
    var4 = var3 + 34;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
