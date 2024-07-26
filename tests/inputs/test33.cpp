#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 60;
  builder::dyn_var<int> var1 = 1;
  builder::dyn_var<int> var2 = 44;
  builder::dyn_var<int> var3 = 77;
  builder::dyn_var<int> var4 = 58;
  while (var3 <= var1) {
    while (var4 < var0) {
      if (var1 != var4) {
      } 
    }
  }
  if (var3 < var0) {
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
