#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 99;
  builder::dyn_var<int> var1 = 23;
  builder::dyn_var<int> var2 = 99;
  builder::dyn_var<int> var3 = 30;
  builder::dyn_var<int> var4 = 6;
  while (var0 > var0) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
