#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 61;
  builder::dyn_var<int> var1 = 23;
  builder::dyn_var<int> var2 = 14;
  builder::dyn_var<int> var3 = 17;
  builder::dyn_var<int> var4 = 66;
  if (var1 > var2) {
    var2 = var1 + 33;
  } else {
    var0 = var2 + 34;
  }
  while (var0 <= var3) {
    if (var1 == var3) {
    } 
  }
  if (var1 > var4) {
    var0 = var0 + 29;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
