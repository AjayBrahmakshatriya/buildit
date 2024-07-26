#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 66;
  builder::dyn_var<int> var1 = 21;
  builder::dyn_var<int> var2 = 74;
  builder::dyn_var<int> var3 = 68;
  builder::dyn_var<int> var4 = 73;
  if (var1 != var0) {
    var1 = var1 + 18;
    if (var0 < var4) {
    } 
    if (var2 != var3) {
      if (var2 != var1) {
        while (var0 >= var3) {
        }
      } else {
        if (var2 <= var2) {
          var4 = var2 + 31;
          var4 = var2 + 55;
        } 
        if (var3 >= var2) {
          var3 = var1 + 97;
        } 
      }
    } 
  } else {
    if (var0 == var1) {
      while (var0 == var3) {
        if (!(var0 <= var0)) {
          if (var0 > var0) {
            var4 = var1 + 91;
          } 
          var2 = var0 + 89;
        } 
      }
      if (var0 >= var2) {
        if (var0 != var1) {
          var1 = var4 + 97;
        } else {
          var0 = var0 + 5;
        }
        var1 = var1 + 17;
        if (var3 == var4) {
          var1 = var0 + 11;
          var1 = var0 + 57;
          var4 = var2 + 37;
        } 
      } 
    } 
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
