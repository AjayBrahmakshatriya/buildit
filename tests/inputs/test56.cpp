#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 20;
  builder::dyn_var<int> var1 = 44;
  builder::dyn_var<int> var2 = 17;
  builder::dyn_var<int> var3 = 20;
  builder::dyn_var<int> var4 = 84;
  if (var0 < var1) {
  } 
  if (var3 >= var2) {
    if (var2 != var0) {
      var1 = var3 + 27;
    } else {
      var1 = var0 + 5;
      if (var0 >= var2) {
        var4 = var0 + 93;
        while (var3 <= var0) {
        }
        while (var4 < var0) {
          var2 = var3 + 8;
        }
      } else {
        if (var3 > var0) {
          var3 = var3 + 13;
          var2 = var0 + 52;
        } else {
          var2 = var3 + 80;
          var2 = var3 + 68;
        }
      }
    }
    if (var2 == var3) {
    } 
  } else {
    var1 = var2 + 36;
    if (var0 != var1) {
      while (var1 < var2) {
        var4 = var0 + 47;
      }
    } 
    var0 = var0 + 53;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
