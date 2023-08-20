#include <memory>
#include <new>

namespace my {

template <typename T>
class Optional {

public:
  // Normal ctor
  Optional() {}

  Optional(const T& value) : has_value_{true}, value_{value} {}

  Optional(T&& value) : has_value_{true}, value_{std::move(value)} {}

  // Dtor
  ~Optional() {
    reset();
  }

  // Universal reference assignment
  template <typename U = T>
  Optional& operator=(U&& other) {
    if (has_value_) {
      value_ = T{std::forward<U>(other)};
    } else {
      construct(std::forward<U>(other));
      has_value_ = true;
    }
    return *this;
  }

  // Construct a new object in place in the optional.
  template <typename... Args>
  T& emplace(Args&&... args) {
    reset();
    has_value_ = true;
    // placement new into the address of the optional type we are holding
    construct(std::forward<Args>(args)...);
    return value_;
  }

  void reset() noexcept {
    if (has_value_) {
      destruct();
    }
    has_value_ = false;
  }

  // Dereference the optional to access the underlying value
  // directly.
  const T& operator*() const& noexcept { return value_; }

  T& operator*() & noexcept { return value_; }

  // To access methods/values of the underlying object.
  const T* operator->() const noexcept { return &value_; }

  T* operator->() noexcept { return &value_; }


private:
  // Placement new construct an object into |value_|.
  // As of C++20, you can use std::construct_at(&value_, std::forward<Args>(args)...);
  template <typename... Args>
  void construct(Args&&... args) {
    ::new(&value_) T{std::forward<Args>(args)...};
  }

  // Placement delete helper method.
  // TODO look into how to do this with placement new
  void destruct() {
    std::destroy_at(&value_);
  }

  bool has_value_{false};
  union {
    // Dummy value so that we can have an "empty" union
    char dummy__;
    T value_;
  };

};

} // namespace my
