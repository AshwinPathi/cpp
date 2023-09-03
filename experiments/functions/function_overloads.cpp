// This question was asked in akuna OA2.

#include <iostream>

struct Base {};
struct Derived : Base {};
struct Leaf final : Derived {
  constexpr operator int() const noexcept {
    return 1;
  }
};

void overload(const Base&) {
  std::cout << "BASE\n";
}

void overload(const Derived&) {
  std::cout << "DERIVED\n";
}

void overload(const int&) {
  std::cout << "INT\n";
}

template <typename T>
void overload(const T&) {
  std::cout << "TEMPLATE\n";
}

int main() {
  // [Pre-answer] I think the answer will be DERIVED.
  Leaf leaf{};
  // The actual answer is TEMPLATE, since that will end up
  // being the most specific overload. If that method didn't
  // exist, then the answer would be DERIVED.
  overload(leaf);
  return 0;
}