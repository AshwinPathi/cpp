#include "optional.h"
#include <iostream>
#include <cassert>

int main() {

  my::Optional<int> x;
  x = 543;
  std::cout << *x << std::endl;

  return 0;
}