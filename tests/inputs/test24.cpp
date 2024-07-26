#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 25;
  builder::dyn_var<int> var1 = 11;
  builder::dyn_var<int> var2 = 51;
  builder::dyn_var<int> var3 = 17;
  builder::dyn_var<int> var4 = 12;
  if (var0 != var4) {
    if (var0 > var4) {
      var2 = var1 + 70;
    } else {
      var4 = var1 + 64;
      while (var3 >= var0) {
        while (var0 > var3) {
        }
        var2 = var0 + 14;
      }
    }
    var1 = var4 + 96;
  } else {
    while (var3 == var2) {
    }
    if (var4 <= var1) {
      var3 = var4 + 71;
      if (var0 < var3) {
      } 
      while (var4 < var2) {
        if (var1 >= var1) {
          var1 = var2 + 69;
        } 
      }
    } 
  }
  if (var0 <= var3) {
    if (var1 != var4) {
      while (var4 <= var2) {
      }
      while (var0 >= var2) {
      }
    } 
    var3 = var1 + 31;
    if (var0 <= var4) {
    } 
  } 
  var0 = var1 + 89;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
