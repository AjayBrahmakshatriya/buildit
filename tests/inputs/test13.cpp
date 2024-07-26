#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 46;
  builder::dyn_var<int> var1 = 13;
  builder::dyn_var<int> var2 = 89;
  builder::dyn_var<int> var3 = 9;
  builder::dyn_var<int> var4 = 45;
  while (var0 <= var4) {
  }
  if (var3 >= var3) {
    while (var4 >= var0) {
      if (var2 < var4) {
      } 
      var1 = var4 + 61;
      var2 = var4 + 25;
    }
  } 
  var4 = var4 + 91;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
