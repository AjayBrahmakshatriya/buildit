#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 33;
  builder::dyn_var<int> var1 = 26;
  builder::dyn_var<int> var2 = 29;
  builder::dyn_var<int> var3 = 2;
  builder::dyn_var<int> var4 = 68;
  while (var1 >= var4) {
  }
  while (var4 == var3) {
  }
  var0 = var1 + 74;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
