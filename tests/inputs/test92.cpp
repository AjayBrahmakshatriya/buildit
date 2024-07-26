#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 60;
  builder::dyn_var<int> var1 = 52;
  builder::dyn_var<int> var2 = 71;
  builder::dyn_var<int> var3 = 40;
  builder::dyn_var<int> var4 = 22;
  if (var3 == var4) {
    var1 = var3 + 45;
  } 
  if (var0 != var0) {
    var0 = var1 + 40;
  } 
  while (var3 != var4) {
    if (var2 < var3) {
      break;
    } else {
      if (var2 == var4) {
        if (var2 != var4) {
          var0 = var3 + 1;
        } 
        while (var2 == var0) {
        }
        break;
      } 
      while (var1 == var2) {
        if (var1 <= var3) {
          var4 = var0 + 40;
          break;
        } else {
          var2 = var1 + 20;
          continue;
          var3 = var4 + 63;
        }
      }
      while (var2 == var0) {
      }
      while (var4 == var0) {
        if (!(var3 >= var0)) {
          if (var0 <= var4) {
          } 
          continue;
        } 
        break;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
