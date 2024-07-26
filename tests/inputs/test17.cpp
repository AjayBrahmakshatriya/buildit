#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 47;
  builder::dyn_var<int> var1 = 20;
  builder::dyn_var<int> var2 = 56;
  builder::dyn_var<int> var3 = 9;
  builder::dyn_var<int> var4 = 99;
  var1 = var0 + 82;
  if (var3 <= var0) {
  } 
  if (var3 > var2) {
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
