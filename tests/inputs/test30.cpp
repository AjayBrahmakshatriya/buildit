#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 0;
  builder::dyn_var<int> var1 = 58;
  builder::dyn_var<int> var2 = 83;
  builder::dyn_var<int> var3 = 49;
  builder::dyn_var<int> var4 = 84;
  while (var4 == var2) {
  }
  while (var3 < var2) {
    var4 = var3 + 74;
    var2 = var0 + 51;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
