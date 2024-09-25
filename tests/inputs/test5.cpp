#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 55;
  builder::dyn_var<int> var1 = 96;
  builder::dyn_var<int> var2 = 66;
  builder::dyn_var<int> var3 = 3;
  builder::dyn_var<int> var4 = 0;
  if (var0 >= var4) {
    var2 = var0 + 57;
    if (var3 > var2) {
      if (var4 >= var3) {
        if (var0 != var3) {
          var3 = var0 + 65;
          var0 = var3 + 70;
        } else {
          var2 = var1 + 12;
          var3 = var2 + 0;
        }
        if (var3 < var2) {
        } 
        var2 = var0 + 89;
      } 
      if (var1 != var2) {
        var2 = var1 + 78;
      } 
    } 
    var2 = var4 + 46;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
