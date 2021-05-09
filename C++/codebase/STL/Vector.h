#ifndef CODEBASE_STL_VECTOR_H_
#define CODEBASE_STL_VECTOR_H_

#include <iostream>
#include <memory>
#include <sstream>

namespace codebase {
template <typename T, typename Alloc = std::allocator<T>>
class Vector {
public:
    using allocator_type = Alloc;
    using value_type = typename allocator_type::value_type;
    using size_type = typename allocator_type::size_type;
    using difference_type = typename allocator_type::difference_type;
    using pointer = typename allocator_type::pointer;
    using const_pointer = typename allocator_type::const_pointer;
    using reference = typename allocator_type::reference;
    using const_reference = typename allocator_type::const_reference;

    Vector();
    Vector(std::initializer_list<value_type>);
    Vector(const Vector<T, Alloc>& rhs);
    Vector<T, Alloc>& operator= (Vector<T, Alloc> rhs);
    ~Vector();

    void push_back(const_reference val);

    size_type size() const { return first_free_ - elements_; }

    size_type capacity() const { return cap_ - elements_; }
    bool empty() const { return size() == 0; }

    void swap(Vector<T, Alloc>& rhs);

    std::string toString(char sep = ' ') const;

private:
    void check_n_alloc();
    void reallocate();
    void free();
    std::pair<pointer, pointer> alloc_n_copy(const_pointer b, const_pointer e);
    void range_initialize(const_pointer b, const_pointer e);

private:
    allocator_type alloc_;
    pointer elements_;
    pointer first_free_;
    pointer cap_;
};

template <typename T, typename Alloc>
Vector<T, Alloc>::Vector() : elements_(nullptr), first_free_(nullptr), cap_(nullptr) {}

template <typename T, typename Alloc>
Vector<T, Alloc>::Vector(std::initializer_list<value_type> il) {
    range_initialize(il.begin(), il.end());
}

template <typename T, typename Alloc>
Vector<T, Alloc>::Vector(const Vector<T, Alloc>& rhs) {
    range_initialize(rhs.elements_, rhs.first_free_);
}

template <typename T, typename Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator= (Vector<T, Alloc> rhs) {
    swap(rhs);
    return *this;
}


template <typename T, typename Alloc>
Vector<T, Alloc>::~Vector() {
    free();
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::push_back(const_reference val) {
    check_n_alloc();
    alloc_.construct(first_free_++, val);
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::check_n_alloc() {
    if (size() == capacity()) {
        reallocate();
    }
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::reallocate() {
    difference_type newcap = size() ? 2 * size() : 1;
    pointer dest = alloc_.allocate(newcap);
    pointer newdata = dest;
    for (pointer src = elements_; src != first_free_; /*空*/) {
        alloc_.construct(dest++, std::move(*src++));
    }
    free();
    elements_ = newdata;
    first_free_ = dest;
    cap_ = elements_ + newcap;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::free() {
    if (elements_) {
        for (pointer tail = first_free_; tail >= elements_; /*空*/) {
            alloc_.destroy(--tail);
        }
        alloc_.deallocate(elements_, cap_ - elements_);
    }
}

template <typename T, typename Alloc>
std::pair<typename Vector<T, Alloc>::pointer, typename Vector<T, Alloc>::pointer> Vector<T, Alloc>::alloc_n_copy(
    const_pointer b, const_pointer e) {
    pointer data = alloc_.allocate(e - b);
    return {data, std::uninitialized_copy(b, e, data)};
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::range_initialize(const_pointer b, const_pointer e) {
    auto p = alloc_n_copy(b, e);
    elements_ = p.first;
    first_free_ = cap_ = p.second;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::swap(Vector<T, Alloc>& rhs) {
    std::swap(elements_, rhs.elements_);
    std::swap(first_free_, rhs.first_free_);
    std::swap(cap_, cap_);
}

template <typename T, typename Alloc>
std::string Vector<T, Alloc>::toString(char sep) const {
    std::stringstream ss;
    ss << '[';

    if (empty()) {
        ss << ']';
        return ss.str();
    }

    pointer curr = elements_;
    ss << *curr;
    ++curr;
    for (; curr != first_free_; ++curr) {
        ss << sep << *curr;
    }
    ss << ']';
    return ss.str();
}


}  // namespace codebase

#endif  // CODEBASE_STL_VECTOR_H_