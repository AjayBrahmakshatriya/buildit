#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 0;
  builder::dyn_var<int> var1 = 39;
  builder::dyn_var<int> var2 = 64;
  builder::dyn_var<int> var3 = 61;
  builder::dyn_var<int> var4 = 15;
  var0 = var2 + 68;
  if (var2 != var4) {
    while (var3 == var2) {
    }
    var2 = var3 + 98;
    var1 = var0 + 66;
  } 
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
