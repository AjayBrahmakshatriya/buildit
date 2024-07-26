#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 22;
  builder::dyn_var<int> var1 = 54;
  builder::dyn_var<int> var2 = 44;
  builder::dyn_var<int> var3 = 10;
  builder::dyn_var<int> var4 = 21;
  while (var0 >= var3) {
  }
  var4 = var0 + 46;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
