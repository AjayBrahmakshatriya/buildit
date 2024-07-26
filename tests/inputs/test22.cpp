#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 3;
  builder::dyn_var<int> var1 = 15;
  builder::dyn_var<int> var2 = 34;
  builder::dyn_var<int> var3 = 4;
  builder::dyn_var<int> var4 = 24;
  if (var0 < var4) {
  } 
  var3 = var3 + 66;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
