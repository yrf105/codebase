#ifndef CODEBASE_STL_LISTNODE_H
#define CODEBASE_STL_LISTNODE_H

#include <memory>

namespace codebase {

template <typename T, typename Alloc=std::allocator<T>> class List;

template <typename ListType, typename Pointer, typename Reference> class ListIterator;

template <typename T>
class ListNode {

private:
    using ListType = List<T, std::allocator<T>>;
    using Alloc = ListType::allocator_type;
    using value_type = typename Alloc::value_type;
    using size_type = typename Alloc::size_type;
    using difference_type = typename Alloc::difference_type;
    using pointer = typename Alloc::pointer;
    using const_pointer = typename Alloc::pointer;
    using reference = typename Alloc::reference;
    using const_reference = typename Alloc::const_reference;

    friend class List<T, std::allocator<T>>;
    friend class ListIterator<ListType, pointer, reference>;
    friend class ListIterator<ListType, const_pointer, const_reference>;
private:

    ListNode(const T& val, ListNode<T>* next = nullptr) : value_(val), next_(next){};
    ~ListNode() {
        delete next_;
    }

    T value_;
    ListNode<T>* next_;
};

}  // namespace codebase

#endif  // CODEBASE_STL_LISTNODE_H