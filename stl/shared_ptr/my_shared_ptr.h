#pragma once
#include <memory>

namespace custom {

template <typename T>
class shared_ptr {
public:
    shared_ptr() : data_{nullptr}, ref_count_{new size_t{0}} {
    }

    explicit shared_ptr(T* ptr) : data_{ptr}, ref_count_{new size_t{1}} {
    }

    // Copy ctors
    shared_ptr(const shared_ptr& other) {
        (*(other.ref_count_))++;
        ref_count_ = other.ref_count_;
        data_ = other.data_;
    }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            cleanup();
            (*(other.ref_count_))++;
            ref_count_ = other.ref_count_;
            data_ = other.data_;
        }
        return *this;
    }
   
    // Move ctors
    shared_ptr(shared_ptr&& other) {
        ref_count_ = other.ref_count_;
        data_ = other.data_;
        other.data_ = nullptr;
        other.ref_count_ = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& other) {
        if (this != &other) {
            cleanup();
            ref_count_ = other.ref_count_;
            data_ = other.data_;
            other.data_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    T& operator*() { return *data_; }
    T& operator->() { return *data_; }

    ~shared_ptr() {
        cleanup();
    }

private:
    void cleanup() {
        if (!ref_count_) {
            return;
        }
        (*ref_count_)--;
        if (*ref_count_ == 0) {
            if (data_) {
                delete data_;
            }
            delete ref_count_;
        }
    }

    T* data_{nullptr};
    size_t* ref_count_;
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    return shared_ptr<T>(new T{std::forward<Args>(args)...});
}

} // namespace custom
