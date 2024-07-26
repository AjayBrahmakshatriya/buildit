#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 53;
  builder::dyn_var<int> var1 = 48;
  builder::dyn_var<int> var2 = 92;
  builder::dyn_var<int> var3 = 28;
  builder::dyn_var<int> var4 = 90;
  var2 = var4 + 92;
  if (var4 == var4) {
    while (var0 != var3) {
      if (var4 > var4) {
        if (var3 != var2) {
        } 
      } else {
        var1 = var2 + 91;
        continue;
      }
      var0 = var3 + 64;
    }
    if (var1 >= var0) {
    } 
    if (var4 == var1) {
      if (var3 != var4) {
        while (var1 == var2) {
          var1 = var1 + 92;
        }
        var2 = var3 + 7;
        if (var2 != var2) {
          var3 = var3 + 97;
        } 
      } else {
        var1 = var0 + 50;
        if (var1 > var2) {
        } 
        var3 = var1 + 44;
      }
      if (var0 > var4) {
        var0 = var3 + 81;
        var3 = var0 + 26;
      } else {
        if (var3 > var3) {
        } 
      }
      while (var4 == var4) {
        while (var2 >= var0) {
          var4 = var2 + 85;
          var4 = var3 + 62;
        }
      }
    } 
  } else {
    var3 = var2 + 5;
    while (var0 < var0) {
      if (var3 <= var2) {
      } 
    }
    if (var4 == var4) {
      if (var1 > var2) {
      } 
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
