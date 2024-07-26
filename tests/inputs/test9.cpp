#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 21;
  builder::dyn_var<int> var1 = 78;
  builder::dyn_var<int> var2 = 7;
  builder::dyn_var<int> var3 = 14;
  builder::dyn_var<int> var4 = 49;
  var1 = var1 + 66;
  while (var2 == var0) {
    var4 = var4 + 69;
    var3 = var4 + 88;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
