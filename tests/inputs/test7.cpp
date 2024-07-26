#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 95;
  builder::dyn_var<int> var1 = 53;
  builder::dyn_var<int> var2 = 19;
  builder::dyn_var<int> var3 = 0;
  builder::dyn_var<int> var4 = 13;
  var4 = var2 + 96;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
