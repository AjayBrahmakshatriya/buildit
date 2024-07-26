#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 62;
  builder::dyn_var<int> var1 = 34;
  builder::dyn_var<int> var2 = 43;
  builder::dyn_var<int> var3 = 8;
  builder::dyn_var<int> var4 = 9;
  if (var4 == var1) {
    var2 = var4 + 80;
    var1 = var2 + 57;
    var0 = var1 + 81;
  } else {
    var3 = var2 + 1;
    while (var1 <= var1) {
      var1 = var0 + 77;
    }
  }
  while (var0 < var4) {
    if (var3 >= var3) {
      while (var0 > var1) {
        var0 = var0 + 50;
      }
      break;
    } else {
      while (var0 != var2) {
        var0 = var4 + 27;
        while (var0 == var3) {
        }
      }
      if (var0 < var1) {
      } 
    }
  }
  if (var0 < var3) {
    if (var3 >= var2) {
      if (var1 != var4) {
        var1 = var2 + 56;
      } else {
        while (var0 > var2) {
        }
      }
      if (var2 > var0) {
      } 
      while (var3 >= var4) {
        var0 = var1 + 62;
      }
    } 
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
