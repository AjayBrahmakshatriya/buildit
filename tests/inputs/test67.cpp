#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 26;
  builder::dyn_var<int> var1 = 46;
  builder::dyn_var<int> var2 = 76;
  builder::dyn_var<int> var3 = 96;
  builder::dyn_var<int> var4 = 2;
  if (var4 > var3) {
    var4 = var3 + 84;
    var2 = var4 + 20;
  } else {
    if (var3 == var1) {
      if (var0 < var3) {
        if (var2 == var3) {
          var2 = var1 + 10;
          var2 = var1 + 65;
        } else {
          var3 = var0 + 78;
          var0 = var1 + 62;
          var4 = var4 + 42;
        }
        if (var2 == var4) {
        } 
      } else {
        if (var4 < var2) {
          var3 = var3 + 87;
          var4 = var3 + 12;
          var3 = var0 + 17;
        } 
        if (var0 <= var1) {
        } 
        if (var1 >= var2) {
          var2 = var4 + 10;
          var3 = var4 + 5;
        } 
      }
      var1 = var3 + 30;
    } else {
      if (var0 <= var1) {
        var2 = var2 + 88;
        if (var2 == var0) {
        } 
        while (var2 > var3) {
        }
      } 
    }
    if (var1 != var3) {
      var1 = var1 + 86;
      var4 = var2 + 70;
    } else {
      while (var4 >= var0) {
      }
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
