#include <random>
#include <iostream>

namespace my {

template <int L, int U>
class RNG {
public:
  RNG() : rng(dev()), dist(L, U) {}

  int random() {
    return dist(rng);
  }

private:
  std::random_device dev;
  std::mt19937 rng;
  std::uniform_int_distribution<std::mt19937::result_type> dist;
};

} // namespace my

int main() {

  my::RNG<0, 50> rng;
  for (int i = 0; i < 100; i++) {
    std::cout << rng.random() << std::endl; 
  }

  return 0;
}
