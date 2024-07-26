#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 37;
  builder::dyn_var<int> var1 = 63;
  builder::dyn_var<int> var2 = 22;
  builder::dyn_var<int> var3 = 47;
  builder::dyn_var<int> var4 = 87;
  while (var1 > var0) {
    while (var0 <= var4) {
      if (var1 >= var3) {
        while (var4 >= var1) {
          var1 = var0 + 84;
        }
        break;
      } else {
        var3 = var2 + 55;
      }
    }
  }
  if (var0 > var4) {
  } 
  if (var2 > var3) {
    if (var2 != var2) {
    } 
    if (var1 <= var0) {
      if (var4 < var2) {
        if (var2 >= var3) {
          var2 = var3 + 64;
          var1 = var3 + 75;
        } 
        while (var4 != var0) {
          var3 = var2 + 5;
        }
      } 
      var2 = var2 + 7;
    } else {
      var1 = var3 + 27;
      while (var4 < var4) {
        if (!(var4 >= var1)) {
          break;
          if (var4 == var2) {
          } 
        } 
      }
    }
    while (var1 == var2) {
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
