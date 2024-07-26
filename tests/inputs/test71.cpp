#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 47;
  builder::dyn_var<int> var1 = 46;
  builder::dyn_var<int> var2 = 71;
  builder::dyn_var<int> var3 = 33;
  builder::dyn_var<int> var4 = 69;
  if (var0 != var0) {
    while (var4 <= var1) {
    }
    if (var3 != var3) {
      if (var1 < var2) {
        var0 = var4 + 43;
      } 
      if (var0 < var2) {
        if (var4 >= var2) {
          var1 = var2 + 13;
        } else {
          var0 = var0 + 64;
        }
      } 
    } 
    var4 = var2 + 66;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
