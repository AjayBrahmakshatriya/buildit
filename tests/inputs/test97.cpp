#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 26;
  builder::dyn_var<int> var1 = 83;
  builder::dyn_var<int> var2 = 90;
  builder::dyn_var<int> var3 = 92;
  builder::dyn_var<int> var4 = 85;
  if (var1 <= var0) {
    if (var4 > var0) {
      while (var4 < var1) {
        var3 = var1 + 61;
      }
      if (var2 <= var2) {
        if (var2 <= var4) {
        } 
      } else {
        if (var3 == var4) {
          var0 = var0 + 48;
          var0 = var3 + 59;
          var1 = var1 + 78;
        } else {
          var0 = var4 + 42;
          var3 = var3 + 80;
        }
        var4 = var0 + 19;
      }
    } 
    if (var1 != var0) {
      var4 = var1 + 68;
      var3 = var3 + 60;
      var3 = var1 + 76;
    } 
  } else {
    while (var0 != var3) {
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
