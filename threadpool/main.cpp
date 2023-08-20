#include "threadpool.h"
#include <iostream>

int main() {
  my::ThreadPool tp(5);

  tp.AddTask([]() {
    std::cout << "writing to stdout on one thread\n";
  });

  tp.AddTask([]() {
    std::cout << "writing to stdout on another thread\n";
  });

  // Make sure we stall here so that all the threads have time to run.
  std::cin.get();
  return 0;
}
