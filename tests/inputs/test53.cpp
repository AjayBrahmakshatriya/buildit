#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 21;
  builder::dyn_var<int> var1 = 25;
  builder::dyn_var<int> var2 = 53;
  builder::dyn_var<int> var3 = 34;
  builder::dyn_var<int> var4 = 45;
  while (var0 >= var1) {
    while (var4 >= var3) {
    }
  }
  while (var0 == var4) {
  }
  while (var1 <= var4) {
    if (var2 <= var0) {
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
