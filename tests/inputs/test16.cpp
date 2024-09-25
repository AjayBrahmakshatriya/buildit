#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 57;
  builder::dyn_var<int> var1 = 51;
  builder::dyn_var<int> var2 = 56;
  builder::dyn_var<int> var3 = 19;
  builder::dyn_var<int> var4 = 49;
  var3 = var1 + 91;
  var2 = var1 + 52;
  if (var3 > var2) {
    if (var0 != var2) {
      if (var0 <= var1) {
        if (var0 > var1) {
        } 
        if (var4 == var0) {
        } 
      } 
      if (var0 != var1) {
        if (var3 < var2) {
          var4 = var0 + 60;
          var1 = var3 + 8;
        } 
      } else {
        var3 = var1 + 39;
        while (var1 >= var3) {
          var4 = var4 + 93;
        }
      }
      while (var1 >= var3) {
        if (var2 == var4) {
          break;
        } 
      }
    } 
    if (var4 == var1) {
      while (var1 != var4) {
        if (var4 != var0) {
          var3 = var3 + 68;
        } else {
          break;
        }
      }
      while (var4 <= var0) {
      }
    } 
    while (var3 >= var0) {
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
