#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 67;
  builder::dyn_var<int> var1 = 55;
  builder::dyn_var<int> var2 = 15;
  builder::dyn_var<int> var3 = 27;
  builder::dyn_var<int> var4 = 33;
  while (var2 > var0) {
    while (var2 < var3) {
      while (var1 <= var0) {
      }
    }
    var1 = var1 + 51;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
