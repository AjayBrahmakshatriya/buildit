#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 60;
  builder::dyn_var<int> var1 = 3;
  builder::dyn_var<int> var2 = 96;
  builder::dyn_var<int> var3 = 98;
  builder::dyn_var<int> var4 = 66;
  var4 = var0 + 72;
  if (var3 > var4) {
    if (var3 != var4) {
      while (var3 <= var4) {
      }
      if (var3 < var4) {
        var3 = var2 + 66;
        var0 = var0 + 9;
        if (var0 <= var1) {
          var2 = var4 + 54;
          var4 = var4 + 49;
        } 
      } 
    } else {
      var3 = var0 + 50;
      var2 = var2 + 34;
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
