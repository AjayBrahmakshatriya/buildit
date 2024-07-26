#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 38;
  builder::dyn_var<int> var1 = 68;
  builder::dyn_var<int> var2 = 10;
  builder::dyn_var<int> var3 = 61;
  builder::dyn_var<int> var4 = 47;
  var2 = var0 + 80;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
