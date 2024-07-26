#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 76;
  builder::dyn_var<int> var1 = 95;
  builder::dyn_var<int> var2 = 85;
  builder::dyn_var<int> var3 = 53;
  builder::dyn_var<int> var4 = 35;
  var0 = var0 + 69;
  var1 = var4 + 3;
  if (var2 > var0) {
    var0 = var4 + 72;
  } else {
    var0 = var3 + 34;
    var4 = var0 + 39;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
