#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 96;
  builder::dyn_var<int> var1 = 99;
  builder::dyn_var<int> var2 = 47;
  builder::dyn_var<int> var3 = 17;
  builder::dyn_var<int> var4 = 92;
  var2 = var1 + 23;
  while (var0 < var3) {
    while (var2 < var3) {
      while (var2 < var0) {
        var4 = var1 + 44;
        if (!(var2 != var3)) {
          break;
        } 
      }
    }
    if (var0 == var1) {
      var4 = var2 + 74;
    } 
    if (var4 <= var0) {
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
