#pragma once
#include <cstddef>
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>

namespace my {

class ThreadPool {
public:
  using Task = std::function<void(void)>;

  // Constructs a thread pool with |num_threads| threads.
  explicit ThreadPool(size_t num_threads);

  // Explicitly specify the destructor.
  ~ThreadPool();

  // Deleted copy constructors since synchronization primitives can't be copied.
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Deleted move ctors since synchronization primitives can't be moved.
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  // Enqueues a new task to the threadpool to be executed asynchronously.
  void AddTask(Task task);

  // Stops the threadpool, and any running tasks.
  void Stop();

  // Getter for the number of threads this threadpool has.
  size_t num_threads() const { return num_threads_; }


private:

  // Takes a task off the threadpool and runs it. This is the main "code" of the
  // threadpool thats run on every thread.
  void loop();

  size_t num_threads_{};
  bool should_stop_{false};
  bool stopped_{false};

  std::vector<std::thread> threads_;

  std::mutex tasks_mutex_;
  std::condition_variable tasks_cv_;

  std::queue<Task> tasks_; // Guarded by |tasks_mutex_|
};

} // namespace my
