#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 95;
  builder::dyn_var<int> var1 = 35;
  builder::dyn_var<int> var2 = 71;
  builder::dyn_var<int> var3 = 79;
  builder::dyn_var<int> var4 = 92;
  var4 = var1 + 41;
  if (var0 <= var2) {
    if (var1 == var2) {
      var4 = var3 + 26;
    } else {
      if (var0 > var1) {
        if (var1 == var2) {
          var3 = var1 + 92;
        } else {
          var3 = var3 + 19;
          var1 = var1 + 6;
        }
        if (var2 >= var3) {
          var3 = var0 + 99;
        } 
        if (var0 == var1) {
          var1 = var3 + 68;
          var0 = var1 + 67;
          var2 = var3 + 4;
        } 
      } 
      if (var2 >= var3) {
        var3 = var3 + 5;
      } else {
        var0 = var2 + 22;
        while (var2 >= var1) {
        }
        if (var3 >= var4) {
          var0 = var4 + 37;
        } else {
          var3 = var0 + 88;
        }
      }
      while (var0 < var4) {
      }
    }
    if (var2 == var1) {
      var3 = var2 + 88;
      if (var4 == var1) {
        var0 = var3 + 50;
        var4 = var0 + 13;
        if (var0 > var3) {
          var2 = var2 + 13;
          var0 = var3 + 11;
          var3 = var1 + 13;
        } 
      } else {
        var3 = var4 + 76;
        var0 = var2 + 56;
      }
      var1 = var0 + 99;
    } else {
      var3 = var4 + 40;
    }
  } 
  while (var3 == var1) {
    while (var1 <= var2) {
      while (var1 >= var2) {
        var1 = var4 + 54;
      }
      if (var4 > var0) {
      } 
    }
    if (var1 > var3) {
      break;
    } else {
      if (var2 > var0) {
      } 
      while (var3 <= var4) {
        var0 = var3 + 99;
        if (var3 < var0) {
        } 
      }
      var0 = var2 + 37;
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
