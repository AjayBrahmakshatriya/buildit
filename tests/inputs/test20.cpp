#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 74;
  builder::dyn_var<int> var1 = 55;
  builder::dyn_var<int> var2 = 24;
  builder::dyn_var<int> var3 = 49;
  builder::dyn_var<int> var4 = 97;
  while (var2 <= var1) {
  }
  var2 = var1 + 70;
  if (var2 < var0) {
    while (var2 <= var4) {
      if (var4 > var1) {
        if (!(var3 > var4)) {
          var1 = var0 + 27;
          if (var4 > var2) {
          } 
        } 
      } 
      if (var4 <= var0) {
        var1 = var1 + 74;
        var1 = var3 + 51;
      } else {
        if (var4 >= var2) {
          continue;
        } 
        while (var3 == var4) {
        }
        var0 = var2 + 89;
      }
    }
    if (var3 != var4) {
      var3 = var4 + 85;
    } else {
      if (var2 == var0) {
        var0 = var0 + 27;
        if (var3 >= var0) {
        } 
      } else {
        if (var1 < var2) {
          var4 = var3 + 45;
        } 
        while (var4 > var0) {
        }
      }
      var0 = var3 + 88;
      var2 = var4 + 49;
    }
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
