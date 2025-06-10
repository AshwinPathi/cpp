#pragma once
#include <memory>
#include <thread>
#include <algorithm>
#include <chrono>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <utility>

namespace my {

template <typename R, typename... Args>
class Scheduler {
 public:
  using Clock = std::chrono::steady_clock;
  using Time = Clock::time_point;
  using Task = std::function<R(Args...)>;
  using TaskID = uint64_t;

  Scheduler() = default;
  ~Scheduler() { stop(); }

  // Starts the scheduler
  void start() {
    std::scoped_lock lck(thread_mutex_, tasks_mutex_);
    if (running_) {
      std::cout << "----- Scheduler already running. -----\n";
      return;
    }

    running_ = true;
    task_runner_ = std::thread([this]() {
      run_task();
    });

    std::cout << "----- Scheduler started at: " << get_seconds(current_time()) << " -----\n";
  }

  // Unschedules all the tasks in the scheduler
  void stop() {
    {
      std::scoped_lock lck(tasks_mutex_);
      if (!running_) {
        std::cout << "----- Scheduler isn't running. -----\n";
        return;
      }
      running_ = false;
      scheduled_ids_.clear();
      tasks_pq_ = std::priority_queue<Schedulable>{};
    }
    tasks_cv_.notify_one();

    std::scoped_lock lck(thread_mutex_);
    if (task_runner_.joinable()) {
      task_runner_.join();
    }
    std::cout << "----- Scheduler stopped. -----\n";
  }

  // Wrapper to schedule a |task| |delay| seconds from now.
  TaskID schedule(size_t delay, Task task, Args... args) {
    auto scheduled_time = current_time() + std::chrono::seconds{delay};
    return schedule(scheduled_time, std::move(task), args...);
  }

  // Schedule a |task| to be executed at |time|. Returns the ID of the scheduled task.
  TaskID schedule(Time time, Task task, Args... args) {
    std::scoped_lock lck(tasks_mutex_);
    Schedulable scheduled_task{std::move(task), std::make_tuple((args)...), time, current_id_};
    std::cout << "----- Scheduling " << scheduled_task.task_id  << " at: " << get_seconds(time) << " -----\n";
    tasks_pq_.emplace(scheduled_task);
    scheduled_ids_.emplace(current_id_);

    if (tasks_pq_.top().task_id == current_id_) {
      tasks_cv_.notify_one();
    }
 
    return current_id_++;
  }

  // Unschedule a task given a |task_id|. Returns true if the task was unscheduled
  // successfully.
  bool unschedule(TaskID task_id) {
    std::scoped_lock lck(tasks_mutex_);

    return scheduled_ids_.erase(task_id) > 0;
  }

 private:
  // An object that can be scheduled and placed in a priority queue.
  struct Schedulable {
    Task task;
    std::tuple<Args...> task_args;
    Time time;
    TaskID task_id;
    bool operator<(const Schedulable& other) const {
      // Priority queue is max heap, so invert this
      return std::tie(time, task_id) > std::tie(other.time, other.task_id);
    }
  };

  void run_task() {
    while (true) {
      std::unique_lock lck(tasks_mutex_);
      if (!running_) {
        break;
      }

      auto wait_time = tasks_pq_.empty() ? max_time() : tasks_pq_.top().time;
      tasks_cv_.wait_until(lck, wait_time);
      // To ensure that we don't schedule tasks we just scheduled, or if stop is called.
      if (!running_ || tasks_pq_.empty() || Clock::now() < tasks_pq_.top().time) {
        continue;
      }

      auto schedulable = tasks_pq_.top();
      tasks_pq_.pop();
      // Lazily don't schedule unscheduled tasks
      if (!scheduled_ids_.count(schedulable.task_id)) {
        continue;
      }
      std::cout << "Running task id: " << schedulable.task_id << " at: " << get_seconds(current_time()) << '\n';
      lck.unlock();
      // Run the task without the mutex
      // schedulable.task();
      std::apply(schedulable.task, schedulable.task_args);
    }
  }

  // Alias for getting the current time
  static Time current_time() { return std::chrono::steady_clock::now(); }

  // Alias for getting the maximum schedulable time (some arbitrarily long time)
  static Time max_time() { return current_time() + std::chrono::seconds{999999}; }

  // Alias for getting the current number of seconds since epoch time. Arbitrary measurement to just get
  // a number from a time point.
  static uint64_t get_seconds(Time time) {
    return std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
  }


  TaskID current_id_{};
  std::atomic<bool> running_;

  // Mutex and CV for the tasks pq and the scheduled IDs
  std::mutex tasks_mutex_;
  std::condition_variable tasks_cv_;

  std::priority_queue<Schedulable> tasks_pq_; // guarded by |tasks_mutex_|
  std::unordered_set<TaskID> scheduled_ids_; // guarded by |tasks_mutex_|

  // Thread to run the scheduled tasks asynchronously from the calling thread.
  // Note that since this is single threaded, long running tasks on the scheduled
  // thread might cause other tasks in the queue to be late.
  std::mutex thread_mutex_;
  std::thread task_runner_; // guarded by |thread_mutex_|



 public:
  // Delete copy/move cotr/assign becauze synchronization structures can't be copied/moved easily.
  Scheduler(const Scheduler&) = delete;
  Scheduler(Scheduler&&) = delete;

  Scheduler& operator=(const Scheduler&) = delete;
  Scheduler& operator=(Scheduler&&) = delete;
};

} // namespace my
