#ifndef CODEBASE_STL_LISTITERATOR_H
#define CODEBASE_STL_LISTITERATOR_H

#include <iterator>

#include "ListNode.h"

namespace codebase {

template <typename ListType, typename Pointer, typename Reference>
class ListIterator {
friend class List<typename ListType::value_type, typename ListType::allocator_type>;
public:
    using value_type = typename ListType::value_type;
    using size_type = typename ListType::size_type;
    using difference_type = typename ListType::difference_type;
    using pointer = Pointer;
    using reference = Reference;

    using link_type = ListNode<value_type>*;
    using iterator_base = ListIterator<ListType, Pointer, Reference>;

    ListIterator() {}
    ~ListIterator() = default;

    reference operator*() { return pointee_->value_; }

    pointer operator->() { return &(pointee_->value_); }

    iterator_base operator++() {
        pointee_ = pointee_->next_;
        return *this;
    }

    iterator_base operator++(int) {
        iterator_base tmp = *this;
        operator++();
        return tmp;
    }

    bool operator==(const iterator_base& rhs) const { return pointee_ == rhs.pointee_; }

    bool operator!=(const iterator_base& rhs) const { return !(*this == rhs); }

private:
    explicit ListIterator(const link_type& pointee) : pointee_(pointee) {}

private:
    link_type pointee_;
};

}  // namespace codebase

#endif  // CODEBASE_STL_LISTITERATOR_H