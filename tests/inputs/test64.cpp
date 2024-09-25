#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 67;
  builder::dyn_var<int> var1 = 75;
  builder::dyn_var<int> var2 = 12;
  builder::dyn_var<int> var3 = 42;
  builder::dyn_var<int> var4 = 34;
  while (var4 > var0) {
    var0 = var1 + 33;
    var2 = var3 + 98;
    var1 = var1 + 25;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
