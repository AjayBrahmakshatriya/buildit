#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 9;
  builder::dyn_var<int> var1 = 36;
  builder::dyn_var<int> var2 = 60;
  builder::dyn_var<int> var3 = 40;
  builder::dyn_var<int> var4 = 53;
  while (var2 != var4) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
