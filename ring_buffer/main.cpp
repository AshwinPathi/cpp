#include "atomic_ring_buffer.h"
#include <iostream>
#include <thread>


int main() {
  constexpr size_t queueSize = 1000;
  const int64_t iters = 100000;

  my::Buffer<int, queueSize> q;
  auto t = std::thread([&] {
    for (int i = 0; i < iters; ++i) {
      int val;
      while (!q.pop(val))
        ;
      if (val != i) {
        throw std::runtime_error("");
      }
    }
  });

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < iters; ++i) {
    while (!q.push(i))
      ;
  }
  while (q.read_pos_.load(std::memory_order_relaxed) !=
         q.write_pos_.load(std::memory_order_relaxed))
    ;
  auto stop = std::chrono::steady_clock::now();
  t.join();
  std::cout << iters * 1000000000 /
                   std::chrono::duration_cast<std::chrono::nanoseconds>(stop -
                                                                        start)
                       .count()
            << " ops/s" << std::endl;

  return 0;
}
