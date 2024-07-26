#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 50;
  builder::dyn_var<int> var1 = 31;
  builder::dyn_var<int> var2 = 42;
  builder::dyn_var<int> var3 = 82;
  builder::dyn_var<int> var4 = 76;
  while (var2 == var0) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
