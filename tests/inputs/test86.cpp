#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 33;
  builder::dyn_var<int> var1 = 30;
  builder::dyn_var<int> var2 = 52;
  builder::dyn_var<int> var3 = 72;
  builder::dyn_var<int> var4 = 96;
  var2 = var0 + 40;
  var3 = var2 + 52;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
