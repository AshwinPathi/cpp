#include "ring_buffer.h"
#include <iostream>

int main() {
  my::Buffer<int, 10> buffer;
  buffer.push(10);

  int x;
  buffer.pop(x);

  std::cout << x << std::endl;

  return 0;
}
