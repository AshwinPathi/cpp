#include "function.h"

#include <iostream>

int add(int a, int b) {
  return a + b;
}

int main() {

  my::Function<int(int, int)> add1(add);

  std::cout << add1(5, 4) << '\n';

  my::Function<int(int, int)> mult([](int a, int b) {
    return a * b;
  });

  std::cout << mult(5, 4) << '\n';


  return 0;
}
