#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 22;
  builder::dyn_var<int> var1 = 24;
  builder::dyn_var<int> var2 = 53;
  builder::dyn_var<int> var3 = 29;
  builder::dyn_var<int> var4 = 82;
  if (var0 <= var4) {
  } 
  if (var3 < var0) {
    var0 = var2 + 80;
  } 
  var1 = var3 + 9;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
