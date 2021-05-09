#ifndef CODEBASE_DS_LIST_H_
#define CODEBASE_DS_LIST_H_

#include <iostream>
#include <sstream>

#include "ListNode.h"
#include "ListIteraor.h"

namespace codebase {

template <typename T, typename Alloc>
class List {
public:
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using size_type = typename Alloc::size_type;
    using difference_type = typename Alloc::difference_type;
    using pointer = typename Alloc::pointer;
    using const_pointer = typename Alloc::pointer;
    using reference = typename Alloc::reference;
    using const_reference = typename Alloc::const_reference;
    using list_type = List<T, Alloc>;
    using iterator = ListIterator<list_type, pointer, reference>;
    using const_iterator = ListIterator<list_type, const_pointer, const_reference>;

    using node = ListNode<value_type>;
    using node_pointer = ListNode<value_type>*;

    explicit List(const allocator_type& alloc = allocator_type()) : allocator_(alloc) { init(); }
    List(const list_type& rhs);
    list_type& operator= (list_type rhs) {
        swap(rhs);
        return *this;
    }
    ~List() {
        delete head_;
    }

    void push_back(const_reference val) {
        node_pointer new_node = create_node(val);
        tail_->next_ = new_node;
        tail_ = tail_->next_;
        ++size_;
    }

    iterator make_iterator(node_pointer np) {
        return iterator(np);
    }

    iterator begin() {
        return make_iterator(head_->next_);
    }

    // iterator end() {
    //     return make_iterator(tail_->next_);
    // }

    void swap(list_type& rhs) {
        std::swap(size_, rhs.size_);
        std::swap(head_, rhs.head_);
        std::swap(tail_, rhs.tail_);
    }

    size_type size() const {
        return size_;
    }

    std::string toString(char sep = ' ') const {
        if (head_ == tail_) {
            return "[]";
        }

        std::stringstream ss;
        ss << '[';
        node_pointer curr = head_->next_;
        while (curr != tail_) {
            ss << curr->value_ << ' ';
            curr = curr->next_;
        }

        ss << curr->value_ << ']';

        return ss.str();
    }

private:
    void init() {
        head_ = create_node(value_type{});
        tail_ = head_;
        size_ = 0;
    }

    node_pointer create_node(const_reference element) {
        // auto new_element = allocator_.allocate(1);
        // allocator_.construct(new_element, element);
        // std::cout << *new_element << std::endl;
        auto new_node = new node(element, nullptr);
        // auto new_node = static_cast<node_pointer>(new_element);
        return new_node;
    }

private:
    size_type size_;
    node_pointer head_;
    node_pointer tail_;
    allocator_type allocator_;
};

template <typename T, typename Alloc>
List<T, Alloc>::List(const list_type& rhs) : List<T, Alloc>() {
    node_pointer curr = rhs.head_->next_;
    while (curr) {
        push_back(curr->value_);
        curr = curr->next_;
    }
}

} // namespace codebase

#endif // CODEBASE_DS_LIST_H_