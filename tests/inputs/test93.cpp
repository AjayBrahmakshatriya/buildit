#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 56;
  builder::dyn_var<int> var1 = 66;
  builder::dyn_var<int> var2 = 52;
  builder::dyn_var<int> var3 = 88;
  builder::dyn_var<int> var4 = 12;
  var0 = var3 + 55;
  var0 = var4 + 53;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
