#include <iostream>
#include <iomanip>

#include "scheduler.h"


int main() {
  my::Scheduler<void, int> sched;
  sched.start();

  sched.schedule(5, [](int x){
    std::cout << "Computed: " << x << '\n';
  }, 435);

  sched.schedule(2, [](int x){
    std::cout << "Computed: " << x << '\n';
  }, 90234);


  // Make sure we stall here so that all the threads have time to run.
  std::cin.get();
  return 0;
}
