#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 58;
  builder::dyn_var<int> var1 = 24;
  builder::dyn_var<int> var2 = 89;
  builder::dyn_var<int> var3 = 28;
  builder::dyn_var<int> var4 = 92;
  if (var1 > var4) {
  } 
  if (var0 <= var2) {
    var3 = var4 + 7;
    if (var0 <= var4) {
      while (var1 <= var2) {
      }
    } 
    if (var3 > var0) {
      var1 = var0 + 63;
    } 
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
