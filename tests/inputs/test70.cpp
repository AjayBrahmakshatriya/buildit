#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 57;
  builder::dyn_var<int> var1 = 58;
  builder::dyn_var<int> var2 = 33;
  builder::dyn_var<int> var3 = 57;
  builder::dyn_var<int> var4 = 53;
  if (var3 > var2) {
    while (var3 != var2) {
      if (var4 > var1) {
        if (var4 != var0) {
          continue;
        } else {
          break;
        }
        while (var1 == var3) {
          var4 = var2 + 14;
        }
      } 
      if (var1 == var2) {
        if (var3 == var2) {
        } 
      } 
    }
    if (var4 == var0) {
      var1 = var0 + 59;
      if (var3 != var2) {
        var2 = var0 + 59;
        if (var3 > var4) {
          var3 = var3 + 11;
        } 
      } else {
        if (var2 <= var0) {
        } 
      }
    } 
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
