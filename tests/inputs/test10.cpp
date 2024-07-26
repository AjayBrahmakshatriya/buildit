#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 62;
  builder::dyn_var<int> var1 = 88;
  builder::dyn_var<int> var2 = 99;
  builder::dyn_var<int> var3 = 52;
  builder::dyn_var<int> var4 = 0;
  var4 = var1 + 99;
  while (var2 == var3) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
