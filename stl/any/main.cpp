#include "any.h"

#include <iostream>
#include <complex>
#include <cassert>

int main() {
  my::Any any(5);

  std::cout << my::any_cast<int>(any) << '\n';

  auto test = my::make_any<std::string>("Test string");
  auto a1 = my::make_any<std::complex<double>>(0.1, 2.3);

  std::cout << my::any_cast<std::string>(test) << '\n';

  assert(any.value());
  any.reset();
  assert(!any.value());

  return 0;
}
