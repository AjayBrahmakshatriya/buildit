#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 23;
  builder::dyn_var<int> var1 = 75;
  builder::dyn_var<int> var2 = 92;
  builder::dyn_var<int> var3 = 13;
  builder::dyn_var<int> var4 = 40;
  if (var3 > var1) {
  } 
  if (var3 > var0) {
    if (var3 >= var1) {
      if (var2 > var0) {
      } 
      var2 = var4 + 38;
    } else {
      var4 = var4 + 11;
      var1 = var4 + 62;
      while (var4 < var1) {
      }
    }
    if (var1 < var2) {
      while (var1 == var3) {
        if (var1 <= var4) {
        } 
      }
      var3 = var2 + 49;
      if (var0 > var3) {
        var3 = var0 + 33;
        if (var0 >= var3) {
          var1 = var0 + 74;
          var3 = var2 + 34;
          var3 = var3 + 88;
        } 
        var1 = var3 + 96;
      } 
    } else {
      if (var0 <= var2) {
      } 
      if (var2 == var4) {
        var3 = var3 + 60;
        var3 = var1 + 65;
      } else {
        while (var2 > var4) {
        }
        if (var4 <= var2) {
          var2 = var2 + 33;
          var1 = var1 + 69;
        } else {
          var3 = var2 + 17;
        }
      }
      var3 = var3 + 87;
    }
    var2 = var1 + 0;
  } else {
    while (var2 < var1) {
    }
    if (var4 >= var0) {
    } 
    if (var1 == var0) {
      if (var1 != var0) {
        if (var4 > var3) {
          var2 = var4 + 41;
          var4 = var2 + 76;
          var2 = var0 + 90;
        } else {
          var4 = var1 + 0;
          var0 = var4 + 94;
          var0 = var1 + 96;
        }
        if (var3 > var0) {
          var0 = var2 + 66;
        } else {
          var4 = var0 + 48;
        }
      } else {
        while (var1 != var0) {
        }
        var0 = var0 + 20;
        var2 = var1 + 52;
      }
      var3 = var3 + 0;
      while (var4 == var0) {
        if (var3 != var4) {
          var0 = var2 + 95;
        } 
        if (!(var0 == var3)) {
          var2 = var0 + 62;
        } 
      }
    } else {
      while (var2 < var0) {
        var2 = var0 + 29;
        var3 = var1 + 5;
        var3 = var0 + 71;
      }
      if (var3 <= var4) {
        var1 = var1 + 37;
        var1 = var1 + 90;
      } 
      while (var4 < var0) {
        if (var4 > var3) {
        } 
      }
    }
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
