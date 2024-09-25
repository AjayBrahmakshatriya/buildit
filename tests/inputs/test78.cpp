#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 71;
  builder::dyn_var<int> var1 = 98;
  builder::dyn_var<int> var2 = 18;
  builder::dyn_var<int> var3 = 46;
  builder::dyn_var<int> var4 = 70;
  if (var3 == var2) {
    if (var4 >= var3) {
      var0 = var2 + 72;
      if (var1 > var2) {
        if (var3 > var0) {
          var2 = var0 + 43;
          var3 = var0 + 78;
        } 
        if (var1 < var4) {
          var0 = var3 + 97;
        } else {
          var2 = var3 + 86;
          var3 = var0 + 33;
          var0 = var3 + 45;
        }
        if (var2 > var0) {
          var1 = var3 + 23;
        } else {
          var4 = var4 + 50;
          var3 = var1 + 40;
        }
      } else {
        while (var1 >= var0) {
          var1 = var0 + 46;
          var4 = var3 + 11;
        }
      }
    } else {
      if (var2 < var4) {
        var1 = var0 + 67;
        if (var0 >= var2) {
        } 
        if (var3 <= var2) {
        } 
      } else {
        if (var3 != var0) {
          var3 = var0 + 37;
          var3 = var0 + 67;
        } else {
          var0 = var1 + 62;
          var0 = var1 + 48;
        }
      }
    }
    while (var4 < var0) {
      if (var1 != var2) {
        if (var2 >= var0) {
        } 
      } 
    }
  } 
  var2 = var2 + 50;
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
