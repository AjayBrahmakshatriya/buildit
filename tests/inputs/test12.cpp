#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 89;
  builder::dyn_var<int> var1 = 45;
  builder::dyn_var<int> var2 = 47;
  builder::dyn_var<int> var3 = 26;
  builder::dyn_var<int> var4 = 48;
  var2 = var3 + 88;
  if (var3 <= var2) {
    var0 = var3 + 26;
    var1 = var2 + 54;
    var0 = var3 + 70;
  } else {
    if (var0 > var1) {
      if (var1 >= var0) {
        if (var0 < var3) {
          var1 = var4 + 33;
          var4 = var3 + 43;
        } else {
          var2 = var2 + 66;
          var3 = var1 + 66;
          var0 = var3 + 41;
        }
        if (var2 < var1) {
          var0 = var2 + 69;
          var3 = var0 + 61;
        } 
        if (var0 > var3) {
          var0 = var3 + 81;
        } 
      } 
    } else {
      var0 = var2 + 42;
      var0 = var2 + 67;
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
