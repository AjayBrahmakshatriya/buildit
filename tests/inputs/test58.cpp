#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 24;
  builder::dyn_var<int> var1 = 81;
  builder::dyn_var<int> var2 = 20;
  builder::dyn_var<int> var3 = 35;
  builder::dyn_var<int> var4 = 2;
  if (var2 >= var2) {
    var4 = var2 + 93;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
