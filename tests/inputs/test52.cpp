#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 60;
  builder::dyn_var<int> var1 = 64;
  builder::dyn_var<int> var2 = 98;
  builder::dyn_var<int> var3 = 42;
  builder::dyn_var<int> var4 = 44;
  while (var0 == var1) {
    var3 = var3 + 89;
    var2 = var2 + 55;
  }
  if (var0 != var3) {
    if (var1 < var0) {
      while (var4 > var0) {
      }
    } 
    while (var1 != var4) {
      while (var0 <= var1) {
      }
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
