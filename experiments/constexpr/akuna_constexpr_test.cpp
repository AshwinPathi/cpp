#include <iostream>
#include <string>
#include <string_view>

constexpr int fn(std::string_view sv) {
  int r = 0;
  for (auto&& c : sv) {
    if (static_cast<int>(c) >= 65 && static_cast<int>(c) <= 90) {
      ++r;
    }
  }
  return r;
}


int main() {
  // This happens at compile time, string literals as string views are
  // constexpr.
  constexpr auto v = fn("AKUNA");
  std::string s;
  std::cin >> s;
  // This happens at runtime, since it uses a runtime variable (s)
  const auto q = fn(s);

  // This happens at compile time, and it has to since integral_constant
  // has to take in constexpr values.
  using G = std::integral_constant<int, fn("Potato Pancakes")>;
  return G::value;
}
