#include <iostream>
#include <string>

#include "tuple.h"

int main() {
  my::Tuple<int, float, std::string> tup{3, 5.0, "test"};

  std::cout << my::get<0>(tup) << '\n';

  std::cout << my::get<0>(my::Tuple<int, float>{3, 3.0}) << '\n';

  return 0;
}
