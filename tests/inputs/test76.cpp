#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 62;
  builder::dyn_var<int> var1 = 36;
  builder::dyn_var<int> var2 = 93;
  builder::dyn_var<int> var3 = 1;
  builder::dyn_var<int> var4 = 12;
  if (var3 > var3) {
    if (var3 >= var0) {
      if (var3 == var3) {
        while (var1 == var2) {
          var2 = var0 + 17;
          var4 = var1 + 30;
          var1 = var3 + 18;
        }
      } 
    } else {
      if (var0 <= var3) {
        while (var4 <= var0) {
        }
      } 
      if (var4 == var4) {
        while (var1 <= var3) {
        }
      } 
    }
  } else {
    var1 = var1 + 79;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
