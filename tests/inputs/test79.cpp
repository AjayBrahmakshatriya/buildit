#include "builder/builder_context.h"
#include "blocks/c_code_generator.h"
void test(void);
void test (void) {
  builder::dyn_var<int> var0 = 97;
  builder::dyn_var<int> var1 = 77;
  builder::dyn_var<int> var2 = 88;
  builder::dyn_var<int> var3 = 48;
  builder::dyn_var<int> var4 = 77;
  if (var1 != var2) {
    while (var3 <= var2) {
      var0 = var0 + 50;
      while (var0 != var1) {
        if (var0 >= var3) {
        } 
      }
    }
  } 
  if (var3 > var1) {
    if (var3 >= var4) {
      while (var0 > var3) {
        var2 = var0 + 13;
      }
      var2 = var3 + 36;
      if (var1 >= var2) {
        if (var2 >= var3) {
          var1 = var1 + 60;
        } 
        while (var1 >= var2) {
          var4 = var0 + 51;
        }
      } 
    } else {
      if (var4 == var0) {
        while (var0 > var1) {
        }
        var2 = var3 + 55;
        if (var0 >= var4) {
          var1 = var4 + 43;
          var0 = var2 + 75;
          var1 = var1 + 76;
        } else {
          var0 = var4 + 96;
          var3 = var0 + 18;
        }
      } 
    }
    if (var0 < var3) {
      var4 = var1 + 85;
      if (var0 <= var4) {
        var2 = var1 + 98;
        var3 = var2 + 50;
        if (var1 <= var3) {
          var2 = var1 + 27;
          var4 = var3 + 70;
          var0 = var0 + 2;
        } else {
          var3 = var2 + 1;
          var3 = var4 + 14;
          var4 = var2 + 51;
        }
      } 
      if (var0 == var2) {
        while (var2 <= var4) {
          var4 = var4 + 56;
          var3 = var4 + 63;
          var3 = var1 + 35;
        }
        var3 = var2 + 35;
      } else {
        if (var1 != var0) {
        } 
        var2 = var4 + 27;
      }
    } 
  } else {
    while (var1 > var2) {
      if (!(var1 > var2)) {
        while (var3 == var1) {
          var4 = var1 + 60;
          var3 = var0 + 84;
        }
        if (var3 < var2) {
          continue;
        } 
        if (var0 == var4) {
        } 
        var4 = var4 + 2;
      } 
      break;
    }
    while (var1 > var2) {
      while (var4 <= var0) {
        if (var4 <= var0) {
        } 
        var2 = var1 + 95;
      }
      if (var1 <= var4) {
        if (var0 != var1) {
        } 
      } else {
        while (var2 > var3) {
          var0 = var1 + 44;
        }
        break;
      }
    }
    if (var0 < var3) {
      if (var0 == var4) {
        while (var3 == var1) {
          var0 = var0 + 94;
        }
        while (var3 < var4) {
          var3 = var0 + 25;
        }
        var4 = var4 + 16;
      } 
      if (var0 >= var1) {
        var2 = var2 + 54;
        var4 = var4 + 45;
        if (var3 <= var2) {
          var3 = var1 + 74;
          var3 = var2 + 34;
        } 
      } else {
        var2 = var1 + 76;
      }
    } else {
      var3 = var2 + 18;
    }
  }
  while (var2 == var0) {
  }
}

int main(int argc, char* argv[]) {
  auto ast = builder::builder_context().extract_function_ast(test, "test");
  block::c_code_generator::generate_code(ast, std::cout, 0);
}
