#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 91;
  builder::dyn_var<int> var1 = 22;
  builder::dyn_var<int> var2 = 81;
  builder::dyn_var<int> var3 = 39;
  builder::dyn_var<int> var4 = 70;
  var0 = var4 + 57;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
