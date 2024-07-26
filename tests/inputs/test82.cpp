#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 48;
  builder::dyn_var<int> var1 = 82;
  builder::dyn_var<int> var2 = 95;
  builder::dyn_var<int> var3 = 53;
  builder::dyn_var<int> var4 = 70;
  if (var4 >= var3) {
    while (var1 < var4) {
    }
  } 
  var0 = var3 + 17;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
