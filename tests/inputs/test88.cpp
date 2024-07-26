#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 36;
  builder::dyn_var<int> var1 = 57;
  builder::dyn_var<int> var2 = 45;
  builder::dyn_var<int> var3 = 24;
  builder::dyn_var<int> var4 = 38;
  var1 = var3 + 92;
  var3 = var0 + 17;
  if (var2 == var3) {
    var4 = var4 + 82;
    while (var4 != var4) {
      if (var3 > var1) {
        if (var1 != var4) {
        } 
        if (var0 > var4) {
          var1 = var2 + 25;
          var0 = var1 + 49;
        } 
      } 
      while (var4 > var2) {
        var1 = var0 + 77;
        while (var0 < var3) {
        }
      }
      if (var1 <= var3) {
        while (var4 <= var1) {
        }
        while (var2 >= var2) {
        }
        while (var1 <= var0) {
        }
      } 
    }
    if (var2 != var3) {
      var4 = var3 + 42;
    } 
  } else {
    if (var1 <= var1) {
      var0 = var3 + 89;
    } else {
      while (var1 != var4) {
      }
      if (var3 != var2) {
        if (var0 != var4) {
          var2 = var0 + 23;
          var1 = var3 + 75;
          var3 = var4 + 11;
        } 
        while (var4 == var4) {
        }
      } else {
        if (var3 != var1) {
        } 
        var0 = var1 + 10;
        var2 = var1 + 88;
      }
      while (var4 > var4) {
        var1 = var3 + 8;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
