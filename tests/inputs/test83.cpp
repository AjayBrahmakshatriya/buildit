#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 69;
  builder::dyn_var<int> var1 = 29;
  builder::dyn_var<int> var2 = 8;
  builder::dyn_var<int> var3 = 45;
  builder::dyn_var<int> var4 = 47;
  if (var2 >= var3) {
    var4 = var0 + 27;
    while (var3 > var0) {
      if (var0 >= var4) {
        while (var1 >= var2) {
        }
        break;
      } 
    }
    while (var2 < var3) {
      var2 = var4 + 54;
      while (var3 > var4) {
      }
      if (!(var0 >= var1)) {
        if (var4 >= var0) {
          break;
        } else {
          var4 = var1 + 79;
          var2 = var3 + 14;
        }
        break;
      } 
    }
  } else {
    if (var2 == var0) {
      if (var4 != var0) {
        var0 = var3 + 27;
      } else {
        var3 = var2 + 96;
      }
      while (var0 >= var1) {
        if (var0 < var3) {
          var1 = var3 + 1;
        } else {
          var0 = var4 + 81;
        }
        if (var2 <= var1) {
        } 
      }
    } 
  }
  while (var3 != var0) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
