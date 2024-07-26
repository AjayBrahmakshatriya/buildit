#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 14;
  builder::dyn_var<int> var1 = 32;
  builder::dyn_var<int> var2 = 20;
  builder::dyn_var<int> var3 = 71;
  builder::dyn_var<int> var4 = 3;
  if (var4 < var0) {
    var3 = var0 + 50;
  } 
  while (var2 != var3) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
