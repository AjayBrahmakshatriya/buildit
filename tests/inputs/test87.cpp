#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 27;
  builder::dyn_var<int> var1 = 32;
  builder::dyn_var<int> var2 = 77;
  builder::dyn_var<int> var3 = 97;
  builder::dyn_var<int> var4 = 52;
  while (var1 < var4) {
    var4 = var2 + 47;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
