#ifndef CODEBASE_DS_LIST_H_
#define CODEBASE_DS_LIST_H_

#include <iostream>
#include <sstream>

namespace codebase {

template <typename T>
struct ListNode {
    ListNode();
    ListNode(const T& val);

    T value;
    ListNode* next;
};

template <typename T>
class List {
public:
    List();
    List(const List<T>& rhs);
    List& operator= (List rhs);
    ~List();

    size_t size() const;
    bool empty() const;
    void push_back(const T& val);
    void pop_back();
    void push_front(const T& val);
    void pop_front();
    void clear();
    void swap(List<T>&);

    std::string toString() const;
private:
    ListNode<T>* head_;
    ListNode<T>* tail_;
    size_t size_;
};

template <typename T>
ListNode<T>::ListNode() : value(T()), next(nullptr) {

}

template <typename T>
ListNode<T>::ListNode(const T& val) : value(val), next(nullptr) {

}

template <typename T>
List<T>::List() : size_(0) {
    head_ = new ListNode<T>;
    tail_ = head_;
}

template <typename T>
List<T>::List(const List<T>& rhs) : head_(rhs.head_), tail_(rhs.tail_), size_(rhs.size_) {

}

template <typename T>
List<T>& List<T>::operator= (List rhs) {
    swap(rhs);
    return *this;
}

template <typename T>
List<T>::~List() {
    clear();
    delete head_;
}

template <typename T>
size_t List<T>::size() const {
    return size_;
}

template <typename T>
bool List<T>::empty() const {
    return size_ == 0;
}

template <typename T>
void List<T>::push_back(const T& val) {
    ListNode<T>* node = new ListNode<T>(val);
    tail_->next = node;
    tail_ = tail_->next;
    ++size_;
}

template <typename T>
void List<T>::pop_back() {
    ListNode<T>* p = head_;
    while (p->next != tail_) {
        p = p->next;
    }
    delete tail_;
    tail_ = p;
    --size_;
    tail_->next = nullptr;
}

template <typename T>
void List<T>::push_front(const T& val) {
    ListNode<T>* node = new ListNode<T>(val);
    node->next = head_->next;
    head_->next = node;
    ++size_;
}

template <typename T>
void List<T>::pop_front() {
    ListNode<T>* p = head_->next;
    head_->next = p->next;
    --size_;
    delete p;
}

template <typename T>
void List<T>::clear() {
    ListNode<T>* curr = head_->next;
    while (curr) {
        head_->next = curr->next;
        delete curr;
        --size_;
        curr = head_->next;
    }
    tail_ = head_;
}

template <typename T>
void List<T>::swap(List<T>& rhs) {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
}


template <typename T>
std::string List<T>::toString() const {
    std::stringstream ss;
    ss << "[";
    ListNode<T>* p = head_->next;
    if (p) {
        ss << p->value;
        p = p->next;
    }

    while (p) {
        ss << "," << p->value;
        p = p->next;
    }
    ss << "]";

    return ss.str();
}
} // namespace codebase

#endif // CODEBASE_DS_LIST_H_