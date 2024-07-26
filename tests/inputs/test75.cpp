#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 89;
  builder::dyn_var<int> var1 = 15;
  builder::dyn_var<int> var2 = 21;
  builder::dyn_var<int> var3 = 59;
  builder::dyn_var<int> var4 = 78;
  while (var4 < var1) {
    var1 = var1 + 49;
  }
  if (var3 < var0) {
    var2 = var3 + 50;
    if (var4 <= var1) {
      while (var0 > var2) {
      }
    } else {
      var4 = var4 + 95;
    }
  } 
  if (var2 < var4) {
    var1 = var1 + 43;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
