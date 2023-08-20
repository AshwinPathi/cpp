#include "threadpool.h"
#include <iostream>

namespace my {

ThreadPool::ThreadPool(size_t num_threads) : num_threads_{num_threads} {
  std::cout << "Starting ThreadPool with: " << num_threads_ << " threads.\n";

  // Reserve instead of resize since we don't really need to default
  // allocate all the threads.  
  threads_.reserve(num_threads_);
  for (int i = 0; i < num_threads_; i++) {
    threads_[i] = std::thread([this]() {
      loop();
    });
  }
}

ThreadPool::~ThreadPool() {
  std::cout << "Destroying ThreadPool.\n";
  if (!stopped_) {
    Stop();
  }
}

void ThreadPool::AddTask(Task task) {
  std::scoped_lock lck(tasks_mutex_);
  tasks_.emplace(std::move(task));
  // Signal to other threads waiting on this condition
  // that a new task has been added to the tasks queue.
  tasks_cv_.notify_one();
}

void ThreadPool::Stop() {
  std::cout << "Stopping ThreadPool.\n";
  {
    // Lock and set the should stop flag so that threads in the worker loop
    // know that they should end looping.
    std::scoped_lock lck(tasks_mutex_);
    should_stop_ = true;
  }
  tasks_cv_.notify_all();
  for (auto& worker : threads_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
  stopped_ = true;
  std::cout << "ThreadPool stopped.\n";
}

void ThreadPool::loop() {
  // The worker loop just loops infinitely looking for tasks to pick up from
  // the |tasks_| queue.
  while (true) {
    std::unique_lock lck(tasks_mutex_);
    // This thread should wait if we have no tasks in the queue and if we haven't
    // stopped. This means we have to wait for a new task to enter. Have to pass in
    // this pointer to lambda so we can use class variables.
    tasks_cv_.wait(lck, [this]() {
      return !tasks_.empty() || should_stop_;
    });
    // If the should stop flag is set, exit the worker loop since we are going
    // to be done with processing.
    if (should_stop_) {
      break;
    }
    Task task = std::move(tasks_.front());
    tasks_.pop();
    // Have to unlock the lock right before we run our task so that we can run
    // other tasks in the threadpool without it being blocked on the task we
    // are running.
    lck.unlock();

    // Run the task from the queue.
    task();
  }
}

} // namespace my
