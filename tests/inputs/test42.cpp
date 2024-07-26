#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 40;
  builder::dyn_var<int> var1 = 60;
  builder::dyn_var<int> var2 = 82;
  builder::dyn_var<int> var3 = 13;
  builder::dyn_var<int> var4 = 11;
  while (var2 >= var4) {
    var0 = var4 + 69;
  }
  if (var3 <= var0) {
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
