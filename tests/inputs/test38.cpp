#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 79;
  builder::dyn_var<int> var1 = 93;
  builder::dyn_var<int> var2 = 78;
  builder::dyn_var<int> var3 = 12;
  builder::dyn_var<int> var4 = 39;
  var1 = var3 + 16;
  while (var0 != var3) {
  }
  if (var1 == var4) {
    if (var3 != var4) {
      if (var0 == var1) {
        var0 = var4 + 81;
      } else {
        if (var4 > var1) {
        } 
        if (var2 == var4) {
          var0 = var4 + 30;
          var0 = var3 + 67;
        } else {
          var4 = var1 + 8;
          var4 = var1 + 43;
        }
      }
      if (var2 < var4) {
      } 
      var1 = var2 + 91;
    } else {
      if (var4 > var0) {
        if (var0 >= var1) {
          var2 = var3 + 80;
        } else {
          var1 = var2 + 4;
          var2 = var3 + 76;
          var2 = var0 + 53;
        }
      } 
    }
    while (var3 <= var2) {
      if (var4 <= var3) {
        var3 = var3 + 44;
        if (var3 != var0) {
        } 
      } 
      var2 = var0 + 50;
    }
  } else {
    var1 = var4 + 50;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
