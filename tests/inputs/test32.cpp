#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 41;
  builder::dyn_var<int> var1 = 17;
  builder::dyn_var<int> var2 = 42;
  builder::dyn_var<int> var3 = 86;
  builder::dyn_var<int> var4 = 30;
  var1 = var3 + 90;
  var1 = var4 + 74;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
