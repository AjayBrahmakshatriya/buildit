#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 97;
  builder::dyn_var<int> var1 = 12;
  builder::dyn_var<int> var2 = 34;
  builder::dyn_var<int> var3 = 49;
  builder::dyn_var<int> var4 = 31;
  while (var2 == var1) {
    var3 = var0 + 62;
  }
  while (var0 > var0) {
    var2 = var2 + 79;
  }
  var2 = var2 + 90;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
