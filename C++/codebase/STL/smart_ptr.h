#ifndef CODEBASE_SMART_PTR_H
#define CODEBASE_SMART_PTR_H

#include <utility>
#include <cstddef>
#include <iostream>

#include "RefCounter.h"

namespace codebase {

template <typename T>
class smart_ptr {
public:
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    smart_ptr(pointer ptr);
    smart_ptr(const_reference val);

    smart_ptr();
    smart_ptr(const smart_ptr& rhs);
    smart_ptr& operator= (smart_ptr rhs);
    ~smart_ptr();

    reference operator*();
    pointer operator->();
    template<typename... Args>
    static smart_ptr make_smart(Args&&... args) {
        return smart_ptr(new value_type(std::forward<Args>(args)...));
    }

    size_type use_count();

    void swap(smart_ptr& rhs);

private:
    pointer pointee_;
    RefCounter counter_;
};

template <typename T>
smart_ptr<T>::smart_ptr(pointer ptr) :  pointee_(ptr) {

}

template <typename T>
smart_ptr<T>::smart_ptr(const_reference val) : pointee_(new value_type(val)) {

}

template <typename T>
smart_ptr<T>::smart_ptr() : pointee_{new value_type} {}

template <typename T>
smart_ptr<T>::smart_ptr(const smart_ptr& rhs) : pointee_(rhs.pointee_), counter_(rhs.counter_) {
    // std::cout << "smart_ptr<T>::smart_ptr(const smart_ptr& rhs)\n";
}

template <typename T>
smart_ptr<T>& smart_ptr<T>::operator= (smart_ptr<T> rhs) {
    swap(rhs);
    return *this;
}

template <typename T>
smart_ptr<T>::~smart_ptr() {
    if (*counter_.count_ == 1) {
        std::cout << "smart_ptr<T>::~smart_ptr()    delete\n";
        delete pointee_;
        pointee_ = nullptr;
    }
    std::cout << "smart_ptr<T>::~smart_ptr()\n";
}

template <typename T>
void smart_ptr<T>::swap(smart_ptr& rhs) {
    using std::swap;
    swap(pointee_, rhs.pointee_);
    // swap(counter_, rhs.counter_);
    counter_.swap(rhs.counter_);
}

template <typename T>
smart_ptr<T>::reference smart_ptr<T>::operator*() {
    return *pointee_;
}

template <typename T>
smart_ptr<T>::pointer smart_ptr<T>::operator->() {
    return pointee_;
}

template <typename T>
smart_ptr<T>::size_type smart_ptr<T>::use_count() {
    return *counter_.count_;
}

} // namespace codebase

#endif // CODEBASE_SMART_PTR_H