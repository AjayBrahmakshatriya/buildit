#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 47;
  builder::dyn_var<int> var1 = 32;
  builder::dyn_var<int> var2 = 74;
  builder::dyn_var<int> var3 = 78;
  builder::dyn_var<int> var4 = 43;
  while (var2 == var1) {
    var2 = var3 + 11;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
