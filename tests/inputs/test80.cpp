#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 64;
  builder::dyn_var<int> var1 = 61;
  builder::dyn_var<int> var2 = 28;
  builder::dyn_var<int> var3 = 6;
  builder::dyn_var<int> var4 = 80;
  if (var3 < var4) {
    while (var0 == var0) {
      if (!(var0 != var1)) {
        break;
      } 
    }
    while (var4 > var0) {
      var4 = var1 + 64;
      var1 = var0 + 2;
      var4 = var2 + 74;
    }
    while (var2 > var4) {
      var3 = var3 + 35;
      while (var4 != var4) {
        if (var0 < var0) {
        } 
        if (var0 <= var2) {
          break;
        } 
      }
      var0 = var3 + 97;
    }
  } else {
    while (var4 > var3) {
      var1 = var1 + 51;
      if (var0 != var3) {
        while (var2 < var0) {
        }
        var1 = var2 + 46;
      } else {
        if (var1 < var1) {
          var4 = var0 + 42;
          var3 = var3 + 78;
          continue;
        } 
      }
      break;
    }
    if (var2 == var3) {
      var1 = var2 + 86;
      if (var2 >= var1) {
        while (var3 >= var2) {
        }
        var1 = var2 + 6;
      } 
    } else {
      var0 = var4 + 5;
    }
    if (var1 <= var0) {
      while (var1 <= var2) {
      }
    } else {
      if (var2 == var2) {
        var1 = var2 + 66;
      } 
      var3 = var1 + 48;
      var1 = var4 + 35;
    }
  }
  var4 = var1 + 92;
  if (var3 >= var1) {
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
