#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 2;
  builder::dyn_var<int> var1 = 20;
  builder::dyn_var<int> var2 = 55;
  builder::dyn_var<int> var3 = 52;
  builder::dyn_var<int> var4 = 35;
  while (var1 == var2) {
    var2 = var1 + 15;
    if (var1 != var4) {
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
