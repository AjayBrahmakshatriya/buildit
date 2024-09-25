#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 37;
  builder::dyn_var<int> var1 = 46;
  builder::dyn_var<int> var2 = 61;
  builder::dyn_var<int> var3 = 62;
  builder::dyn_var<int> var4 = 30;
  var1 = var3 + 18;
  if (var1 <= var3) {
    while (var2 < var1) {
      if (var0 == var1) {
      } 
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
