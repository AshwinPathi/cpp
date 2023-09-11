#pragma once
#include <memory> // for default delete

#ifdef DEBUG
#include <iostream>
#endif

namespace custom {

template<class T, class Deleter = std::default_delete<T>>
class unique_ptr {
    public:
        // constructors
        unique_ptr() : data_(nullptr) {
        #ifdef DEBUG
            std::cout << "Nullptr constructor" << std::endl;
        #endif
        }
        explicit unique_ptr(T* data) : data_(data) {
        #ifdef DEBUG
            std::cout << "Basic constructor" << std::endl;
        #endif
        }

        // Move constructors
        unique_ptr(unique_ptr&& other) noexcept {
        #ifdef DEBUG
            std::cout << "Move constructor" << std::endl;
        #endif
            cleanup();
            data_ = other.data_;
            other.data_ = nullptr;
        }

        unique_ptr& operator=(unique_ptr&& other) noexcept {
        #ifdef DEBUG
            std::cout << "Move assignment" << std::endl;
        #endif
            if (this == &other) {
                return *this;
            }
            cleanup();
            data_ = other.data_;
            other.data_ = nullptr;
            return *this;
        }

        // Unique pointers cannot be copied. This means no
        // copy assignment or copy constructor.
        unique_ptr(unique_ptr const&)            = delete;
        unique_ptr& operator=(unique_ptr const&) = delete;

        // destructor
        ~unique_ptr() {
        #ifdef DEBUG
            std::cout << "Destructor called" << std::endl;
        #endif
            cleanup(); 
        }

        // Various operators (dereference, pointer, etc.)
        T* operator->() const { return data_; }
        T& operator*()  const { return *data_; }
        explicit operator bool() const { return data_; }

        T* get()        const { return data_; }


    private:
        void cleanup() {
            if (data_ == nullptr) {
                return;
            }
            Deleter()(data_);
        }

        T* data_;
};


} // namespace custom
