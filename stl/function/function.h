#pragma once
#include <memory>
#include <type_traits>

namespace my {

template <typename R>
class Function;

template <typename R, typename... Args>
class Function<R(Args...)> {

public:
  template <typename F>
  Function(F&& function) : function_ptr_{std::make_unique<TypeErasedContainerDerived<std::decay_t<F>>>(std::forward<F>(function))} {}

  R operator()(Args&&... args) const {
    return function_ptr_->invoke(std::forward<Args>(args)...);
  }

  explicit operator bool() const noexcept {
    return function_ptr_ != nullptr;
  }

  const std::type_info& type() const noexcept {
    return function_ptr_->type();
  }

private:
  struct TypeErasedContainerBase {
    virtual ~TypeErasedContainerBase() = default;
    // Type erased functions have to implement the invoke() method, which calls
    // the type erased function.
    virtual R invoke(Args&&... args) const = 0;
    // Get the underlying type of the type erased function.
    virtual const std::type_info& type() const noexcept = 0;
  };

  template <typename F>
  struct TypeErasedContainerDerived : TypeErasedContainerBase {
    F underlying_function_;

    TypeErasedContainerDerived(F&& underlying_function) : underlying_function_{std::forward<F>(underlying_function)} {}

    R invoke(Args&&... args) const override {
      return underlying_function_(std::forward<Args>(args)...);
    }

    const std::type_info& type() const noexcept override {
      return typeid(F);
    }
  };

  std::unique_ptr<TypeErasedContainerBase> function_ptr_;
};

} // namespace my
