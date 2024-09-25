#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 17;
  builder::dyn_var<int> var1 = 82;
  builder::dyn_var<int> var2 = 61;
  builder::dyn_var<int> var3 = 75;
  builder::dyn_var<int> var4 = 42;
  while (var1 > var2) {
    if (!(var3 == var0)) {
      break;
    } 
  }
  var0 = var1 + 8;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
