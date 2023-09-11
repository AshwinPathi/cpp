#include <iostream>
#include <iomanip>

#include "scheduler.h"


int main() {
  my::Scheduler sched;
  sched.start();

  sched.schedule([](){
    int x = 0;
    for (int i = 0; i < 10000; i++) {
      x += i * 3;
    }
    std::cout << "Computed: " << x << '\n';
  }, 5);

  sched.schedule([](){
    int x = 0;
    for (int i = 0; i < 10000; i++) {
      x += (i - 3);
    }
    std::cout << "Computed: " << x << '\n';
  }, 2);


  // Make sure we stall here so that all the threads have time to run.
  std::cin.get();
  return 0;
}
