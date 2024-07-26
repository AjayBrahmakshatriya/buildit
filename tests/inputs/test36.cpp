#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 6;
  builder::dyn_var<int> var1 = 34;
  builder::dyn_var<int> var2 = 17;
  builder::dyn_var<int> var3 = 7;
  builder::dyn_var<int> var4 = 47;
  if (var2 <= var0) {
    while (var1 >= var3) {
    }
    var3 = var1 + 26;
    var0 = var4 + 39;
  } else {
    var3 = var1 + 50;
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
