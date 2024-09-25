#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 89;
  builder::dyn_var<int> var1 = 33;
  builder::dyn_var<int> var2 = 97;
  builder::dyn_var<int> var3 = 46;
  builder::dyn_var<int> var4 = 88;
  if (var4 < var0) {
    if (var1 != var3) {
      var3 = var3 + 53;
    } 
    while (var4 < var0) {
      if (var2 <= var4) {
      } 
    }
  } 
  while (var4 == var3) {
    var1 = var3 + 28;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
