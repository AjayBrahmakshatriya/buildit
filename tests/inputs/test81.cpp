#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 99;
  builder::dyn_var<int> var1 = 87;
  builder::dyn_var<int> var2 = 83;
  builder::dyn_var<int> var3 = 11;
  builder::dyn_var<int> var4 = 24;
  var3 = var1 + 7;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
