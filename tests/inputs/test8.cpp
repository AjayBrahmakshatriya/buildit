#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 9;
  builder::dyn_var<int> var1 = 47;
  builder::dyn_var<int> var2 = 82;
  builder::dyn_var<int> var3 = 36;
  builder::dyn_var<int> var4 = 56;
  while (var4 == var4) {
    while (var4 <= var4) {
      var0 = var0 + 47;
      var3 = var2 + 58;
    }
    while (var3 <= var3) {
      if (var4 < var1) {
      } 
    }
  }
  var1 = var1 + 29;
  if (var1 == var1) {
    while (var1 >= var3) {
      var1 = var1 + 52;
    }
    var3 = var2 + 8;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
