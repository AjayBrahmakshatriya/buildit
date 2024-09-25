#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 1;
  builder::dyn_var<int> var1 = 34;
  builder::dyn_var<int> var2 = 22;
  builder::dyn_var<int> var3 = 28;
  builder::dyn_var<int> var4 = 74;
  if (var3 < var1) {
    var3 = var4 + 70;
  } 
  var2 = var2 + 95;
  if (var2 > var0) {
    if (var3 > var1) {
      while (var3 == var4) {
        if (var4 >= var2) {
        } 
      }
      var3 = var4 + 1;
    } 
    if (var3 > var1) {
      if (var2 <= var0) {
        while (var4 == var2) {
        }
        if (var0 != var1) {
          var0 = var0 + 72;
          var1 = var4 + 91;
        } 
      } else {
        var2 = var0 + 9;
        if (var0 == var1) {
        } 
      }
      var4 = var1 + 64;
      while (var3 >= var4) {
        if (!(var1 >= var2)) {
          break;
        } 
      }
    } 
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
