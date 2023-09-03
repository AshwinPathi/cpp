#include <iostream>
#include <mutex>
#include <thread>


struct Datum final {
  int x;
  int y;
  std::mutex xlock;
  std::mutex ylock;
};


Datum global{0, 0, {}, {}};

void RoutineA() {
  std::lock_guard<std::mutex> g1{global.xlock};
  std::lock_guard<std::mutex> g2{global.ylock};
  ++global.x;
  ++global.y;
}

void RoutineB() {
  std::lock_guard<std::mutex> g1{global.ylock};
  std::lock_guard<std::mutex> g2{global.xlock};
  --global.y;
  --global.x;
}

int main() {
  /* This has undefined behavior. There are two possibilities:
  1. we deadlock (since the lock ordering is ill defined)
  2. we get 0

  There are no other possible outcomes.

  We can solve a deadlock by using a scoped lock and using both the locks
  to avoid deadlocking.
  */
  std::thread t1{RoutineA};
  std::thread t2{RoutineB};

  t1.join();
  t2.join();

  std::cout << global.x << " | " << global.y << std::endl;

  return 0;
}