#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 19;
  builder::dyn_var<int> var1 = 68;
  builder::dyn_var<int> var2 = 16;
  builder::dyn_var<int> var3 = 88;
  builder::dyn_var<int> var4 = 50;
  if (var3 != var1) {
    if (var2 != var1) {
    } 
    while (var0 <= var1) {
      if (var1 > var2) {
        while (var0 != var4) {
        }
      } else {
        break;
      }
    }
  } 
  var2 = var4 + 92;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
