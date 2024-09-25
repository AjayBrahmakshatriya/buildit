#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 6;
  builder::dyn_var<int> var1 = 58;
  builder::dyn_var<int> var2 = 63;
  builder::dyn_var<int> var3 = 33;
  builder::dyn_var<int> var4 = 49;
  var3 = var2 + 48;
  if (var2 != var4) {
    var2 = var1 + 57;
    if (var2 <= var0) {
      while (var2 != var3) {
        if (var4 <= var3) {
        } 
        if (!(var3 != var2)) {
          break;
        } 
      }
    } 
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
