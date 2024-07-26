#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 8;
  builder::dyn_var<int> var1 = 30;
  builder::dyn_var<int> var2 = 87;
  builder::dyn_var<int> var3 = 49;
  builder::dyn_var<int> var4 = 20;
  var2 = var3 + 21;
  if (var0 != var3) {
    var2 = var2 + 45;
    if (var0 == var1) {
      var2 = var2 + 81;
      while (var4 > var1) {
        if (var4 >= var3) {
        } 
      }
    } 
    var4 = var4 + 87;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
