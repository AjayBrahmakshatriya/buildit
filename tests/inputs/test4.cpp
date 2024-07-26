#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 17;
  builder::dyn_var<int> var1 = 52;
  builder::dyn_var<int> var2 = 42;
  builder::dyn_var<int> var3 = 74;
  builder::dyn_var<int> var4 = 30;
  var3 = var4 + 3;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
