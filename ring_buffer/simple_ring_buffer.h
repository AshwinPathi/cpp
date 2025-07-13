#include <array>

namespace my {

template<typename T, size_t N>
class RingBuffer {
private:
    std::array<T, N> buffer;
    size_t head;     // Points to next write position
    size_t tail;     // Points to next read position
    size_t count;    // Current number of elements
    static constexpr size_t capacity = N;

public:
    RingBuffer() : head(0), tail(0), count(0) {
        static_assert(N > 0, "Buffer size must be greater than 0");
    }

    // Push an element to the buffer
    void push(const T& item) {
        if (count == capacity) {
            // Buffer is full, overwrite oldest element
            tail = (tail + 1) % capacity;
        } else {
            count++;
        }
        
        buffer[head] = item;
        head = (head + 1) % capacity;
    }

    // Pop an element from the buffer
    T pop() {
        if (count == 0) {
            throw std::underflow_error("Buffer is empty");
        }
        
        T item = buffer[tail];
        tail = (tail + 1) % capacity;
        count--;
        
        return item;
    }

    // Get current size of the buffer
    size_t size() const {
        return count;
    }

    // Check if buffer is empty
    bool empty() const {
        return count == 0;
    }

    // Check if buffer is full
    bool full() const {
        return count == capacity;
    }

    // Get maximum capacity
    size_t max_size() const {
        return capacity;
    }
};

} // namespace my
