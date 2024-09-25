#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 69;
  builder::dyn_var<int> var1 = 33;
  builder::dyn_var<int> var2 = 65;
  builder::dyn_var<int> var3 = 77;
  builder::dyn_var<int> var4 = 46;
  if (var0 < var1) {
  } 
  while (var2 <= var3) {
    var4 = var1 + 86;
    if (var1 <= var2) {
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
