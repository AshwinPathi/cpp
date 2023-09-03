#include <iostream>

struct Dummy {};


// Which of these functions will only take int?
void foo(int) {
  std::cout << "foo(int)\n";
}

template <typename T>
void foo2(T) {
  std::cout << "foo2(T)\n";
}

template <typename T>
void foo3(T) {
  static_assert(std::is_same_v<T, int>);
  std::cout << "foo3(T)\n";
}

template <typename T>
auto foo4(T) -> std::enable_if_t<std::is_same_v<T, int>, void> {
  std::cout << "foo4(T)\n";
}

/* Can't do this one unless i have c++20
void foo5(std::is_same_v<int> auto) {
  std::cout << "foo5(T)\n";
}
*/

class Foo {
  explicit Foo(int) {}
};

int main() {
  int a = 4;
  float f = 3.0;
  Dummy d{};


  // Foo --> Doesn't only work with int
  foo(a); // Can bind to int fine
  foo(f); // Can also bind to floats, doubles, etc. any type convertible to int
  // foo(d); can only bind to numeric types, but not random structs


  // Foo2 --> Doesn't only work with int
  foo2(a); // Works with int
  foo2(d); // But can be invoked with any type


  // Foo3 --> Only work with int
  foo3(a); // Works with int
  // foo3(f); Won't work since static assert will fail, float != int
  // foo(d); Doesn't work with arbitrary object


  // Foo4 --> Only work with int
  foo4(a); // Works with int
  // foo4(f); Doesn't work with float, since float != int
  // foo4(d); Doesn't work with arbitrary object, since Dummy != int

  return 0;
}