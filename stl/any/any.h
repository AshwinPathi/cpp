#pragma once
#include <memory>
#include <type_traits>

namespace my {

class Any {
public:
  // Ctor, this is trivial if we don't provide anything.
  constexpr Any() noexcept = default;
  // Dtor, this is trivial since we don't need to do anything special.
  ~Any() = default;

  // Any ctor taking in a universal reference so that we can construct from any generic
  // object. Note that the type T has to be copy constructable.
  // Since this pattern is used a few times in the code, I'll explain it here.
  // When we create a new std::any, we first have to wrap our object in a pointer
  // to allow us to have an extra layer of indirection for type erasure.
  // There are a few components to this:
  // - std::make_unique<TypeErasedContainerDerived< ....... >>
  //    - This is the "type erause" component of the any, where we set the base ptr
  //      to the derived ptr.
  // - unique_ptr<...>(std::forward<T>(value))
  //    - The forwarding allows the universal reference 
  // - std::decay_t<T>
  //    - Decays the type to pointers or pointers to functions.
  template <typename T>
  Any(T&& value) : any_ptr_{any_ptr_ = std::make_unique<TypeErasedContainerDerived<std::decay_t<T>>>(std::forward<T>(value))} {}

  // In place constructor.
  template<typename T, typename... Args>
  explicit Any(std::in_place_type_t<T>, Args&&... args) :
    any_ptr_{std::make_unique<TypeErasedContainerDerived<std::decay_t<T>>>(std::forward<Args>(args)...)} {}

  // Any copy assignment taking in a universal reference so that we can construct from any generic
  // object. Note that the type T has to be copy constructuble. You can enforce this
  // with an additional template argument:
  // typename = std::enable_if_t<std::is_copy_constructible_v<std::decay_t<T>>>
  template <typename T>
  Any& operator=(T&& value) {
    any_ptr_ = std::make_unique<TypeErasedContainerDerived<std::decay_t<T>>>(std::forward<T>(value));
    return *this;
  }

  ////// copy/move ctor/assignment from _other_ any types.
  // Copy ctor
  Any(const Any& other) : any_ptr_{other.any_ptr_->clone()} {}

  // Copy assignment
  Any& operator=(const Any& other) {
    any_ptr_ = other.any_ptr_->clone();
    return *this;
  }

  // Move ctor
  Any(Any&& other) noexcept : any_ptr_{std::move(other.any_ptr_)} {
    other.any_ptr_ = nullptr;
  }

  // Move assignment
  Any& operator=(Any&& other) noexcept {
    any_ptr_ = std::move(other.any_ptr_);
    other.any_ptr_ = nullptr;
    return *this;
  }

  // Change the contained object and directly construct the new
  // object in its place. T is supposed to be copy constructuble and the Args
  // are supposed to be constructible, but I omitted them here.
  template <typename T, typename... Args>
  void emplace(Args&&... args) {
    any_ptr_ = std::make_unique<TypeErasedContainerDerived<std::decay_t<T>>>(std::forward<Args>(args)...);
  }

  // Destroys the contained object
  void reset() noexcept {
    any_ptr_.reset();
  }

  // Swaps two any objects
  void swap(Any& other) noexcept {
    std::swap(any_ptr_, other.any_ptr_);
  }

  // Gets the current type of the any ptr.
  const std::type_info& type() const noexcept {
    return any_ptr_->type();
  }

  // Checks to see if the any ptr has a value.
  const bool value() const noexcept {
    return any_ptr_ != nullptr;
  }

private:
  // The type erased constructs. The base requires each derived type erased
  // object to implement their type and a clone method.
  struct TypeErasedContainerBase {
    virtual ~TypeErasedContainerBase() = default;
    virtual const std::type_info& type() const noexcept = 0;
    virtual std::unique_ptr<TypeErasedContainerBase> clone() = 0;
  };

  template <typename T>
  struct TypeErasedContainerDerived : TypeErasedContainerBase {
    // Captures the underlying value for the std any behind a type erased
    // container.
    T underlying_value_;

    // Dtor is default and inherited from the base.

    // This is almost like emplace from earlier, its to account for types that require
    // some variadic argument list to initialize it.
    template <typename... Args>
    TypeErasedContainerDerived(Args... args) : underlying_value_{std::forward<Args>(args)...} {}

    // Just return the underlying type information.
    const std::type_info& type() const noexcept override {
      return typeid(T);
    }

    // Make a clone of the underlying type thru make unique.
    std::unique_ptr<TypeErasedContainerBase> clone() override {
      return std::make_unique<TypeErasedContainerDerived<T>>(underlying_value_);
    }
  };

  // This is where the magic is. The pointer that any holds is _type erased_, meaning
  // the pointer points to a base object, but the initialized objects in the any_ptr are
  // Derived objects. This way, we cna store arbitrary objects in the ptr using an
  // extra layer of indirection, and offer the same interface for any object regardless
  // of what type it is.
  std::unique_ptr<TypeErasedContainerBase> any_ptr_;

  // Any cast friend class
  template <typename T>
  friend T any_cast(const Any& any);

  // Make any friend class
  template<typename T, typename... Args>
  friend Any make_any( Args&&... args );
};

template <typename T>
T any_cast(const Any& any) {
  // Attempt to cast the inputted Any object to the 
  auto* casted_ptr = dynamic_cast<Any::TypeErasedContainerDerived<T>*>(any.any_ptr_.get());
  if (casted_ptr == nullptr) {
    throw std::bad_cast();
  }
  return casted_ptr->underlying_value_;
}

template<typename T, typename... Args>
Any make_any(Args&&... args) {
  return Any{std::in_place_type<T>, std::forward<Args>(args)...};
}

} // namespace my
