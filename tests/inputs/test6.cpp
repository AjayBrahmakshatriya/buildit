#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 57;
  builder::dyn_var<int> var1 = 45;
  builder::dyn_var<int> var2 = 25;
  builder::dyn_var<int> var3 = 28;
  builder::dyn_var<int> var4 = 85;
  var0 = var2 + 85;
  if (var1 >= var2) {
    var1 = var1 + 90;
    if (var4 < var2) {
      if (var4 >= var2) {
        var3 = var0 + 95;
        if (var3 > var1) {
        } 
      } 
      if (var4 >= var0) {
        while (var3 < var4) {
        }
        while (var2 <= var0) {
          var0 = var2 + 81;
        }
      } else {
        while (var3 == var4) {
          var0 = var4 + 22;
        }
        while (var0 != var2) {
        }
      }
    } 
    if (var1 < var2) {
      var0 = var0 + 5;
      var4 = var0 + 37;
    } else {
      while (var1 >= var2) {
      }
      if (var4 > var1) {
        var3 = var0 + 47;
        var2 = var3 + 40;
      } else {
        while (var0 < var1) {
          var0 = var1 + 23;
        }
      }
      while (var3 < var2) {
        if (var4 == var3) {
        } 
        var2 = var2 + 37;
      }
    }
  } else {
    if (var2 >= var1) {
    } 
  }
  if (var1 <= var2) {
    while (var3 < var0) {
      if (var2 == var0) {
        var2 = var0 + 0;
      } 
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
