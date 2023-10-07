// atomic lock free ring buffer: https://rigtorp.se/ringbuffer/
// https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Acquire_ordering
#include <array>
#include <atomic>
#include <new>

namespace {

// normally this would be std::hardware_destructive_interference_size
constexpr size_t cache_line_size = 64; // bytes

} // namespace

namespace my {

template <typename T, size_t N>
class Buffer {
public:
  Buffer() = default;
  ~Buffer() = default;

  bool push(const T& elem) {
    const auto write_idx = write_pos_.load(std::memory_order_relaxed);
    auto next_write_idx = write_idx + 1;
    if (next_write_idx == data_.size()) {
      next_write_idx = 0;
    }
    // If the next position to write is the read position, then we know the buffer is full.
    // After this load, the thread calling push() is guaranteed to see all the changes made
    // to the buffer that happened BEFORE the read_pos_ store in pop().
    if (next_write_idx == read_pos_.load(std::memory_order_acquire)) {
      return false;
    }
    data_[write_idx] = elem;
    write_pos_.store(next_write_idx, std::memory_order_release);
    return true;
  }

  bool pop(T& returned_elem) {
    const auto read_idx = read_pos_.load(std::memory_order_relaxed);
    // Once again, if the read and write indices are the same, then we know the buffer is full.
    // After this load, the thread calling pop() is guaranteed to see all the changes made
    // to the buffer that happened BEFORE the write_pos_ store in push().
    if (read_idx == write_pos_.load(std::memory_order_acquire)) {
      return false;
    }
    returned_elem = std::move(data_[read_idx]);
    auto next_read_idx = read_idx + 1;
    if (next_read_idx == data_.size()) {
      next_read_idx = 0;
    }
    read_pos_.store(next_read_idx, std::memory_order_release);
    return true;
  }

public:
  std::array<T, N> data_;
  // Align read and write pos to cache line to reduce cache coherency traffic.
  alignas(cache_line_size) std::atomic<size_t> read_pos_{};
  alignas(cache_line_size) std::atomic<size_t> write_pos_{};
};

} // namespace my
