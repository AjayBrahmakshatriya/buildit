#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 87;
  builder::dyn_var<int> var1 = 74;
  builder::dyn_var<int> var2 = 24;
  builder::dyn_var<int> var3 = 45;
  builder::dyn_var<int> var4 = 14;
  if (var1 != var2) {
    while (var1 == var4) {
      var0 = var4 + 62;
    }
  } else {
    var0 = var2 + 30;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
