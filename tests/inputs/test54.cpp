#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 83;
  builder::dyn_var<int> var1 = 16;
  builder::dyn_var<int> var2 = 70;
  builder::dyn_var<int> var3 = 95;
  builder::dyn_var<int> var4 = 79;
  while (var2 < var4) {
    var1 = var0 + 49;
  }
  while (var4 <= var3) {
    if (var4 == var1) {
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
