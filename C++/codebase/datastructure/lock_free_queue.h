#ifndef CODEBASE_DS_LOCK_FREE_QUEUE
#define CODEBASE_DS_LOCK_FREE_QUEUE

#include <sstream>
#include <iostream>

namespace codebase {

template <typename T>
struct QueueNode {
    QueueNode();
    QueueNode(const T& val);

    T value;
    QueueNode<T>* next;
};

template <typename T>
QueueNode<T>::QueueNode() : value(), next(nullptr) {}

template <typename T>
QueueNode<T>::QueueNode(const T& val) : value(val), next(nullptr) {}

template <typename T>
class LockFreeQueue {
public:
    LockFreeQueue();

    ~LockFreeQueue();

    void push(const T& val);
    T pop();
    void clear();

    std::string toString() const;

private:
    QueueNode<T>* head_;
    QueueNode<T>* tail_;
};

template <typename T>
LockFreeQueue<T>::LockFreeQueue() {
    head_ = new QueueNode<T>();
    tail_ = head_;
}

template <typename T>
LockFreeQueue<T>::~LockFreeQueue() {
    QueueNode<T>* p = head_->next;
    while (p) {
        head_->next = p->next;
        delete p;
        p = head_->next;
    }

    delete head_;
}

// template <typename T>
// void LockFreeQueue<T>::push(const T& val) { // 入队
//     QueueNode<T>* node = new QueueNode<T>(val); // 构造入队节点
//     QueueNode<T>* p = nullptr;
//     do {
//         p = tail_;  // 获得队尾节点的快照

//         /**--------------------------- 改进永远拿不到 tail v1 ----------------------
//          *
//         */
//         while (p->next = tail_) {
//             p = p->next;
//         }

//         // CAS：如果 p 真的是队尾指针（即，拿到 tail_ 后没有别的线程先于自己在队尾插入元素），
//         // 则 p->next 就一定是 nullptr，那就将 node 赋值给 p->next
//         // 如果 p->next 不是 nullptr 则说明有线程在自己之前在队尾插入了元素，
//         // CAS 会 返回 false，重试
//     } while (__sync_bool_compare_and_swap(&(p->next), nullptr, node) == false);

//     // 这一步不需要重试，因为如果本线程拿到了真正的队尾节点，其他线程就一定拿不到。
//     // 因为，在改变 tail_ 指向新插入节点之前，tail_->next 一定不为空
//     // 这里存在隐患，若拿到 tail_ 的线程在设置 tail_ 之前崩溃了，那其他线程就永远拿不到 tail_ 了
//     __sync_bool_compare_and_swap(&tail_, p, node);
   
// }

/**
 *--------------------- 改进永远拿不到 tail v1 ---------------------
*/
// template <typename T>
// void LockFreeQueue<T>::push(const T& val) { // 入队
//     QueueNode<T>* node = new QueueNode<T>(val); // 构造入队节点
//     QueueNode<T>* p = tail_;
//     do {
//         // 每个线程在获取失败后，都主动寻找 队尾 ，而不是直接拿 tail_
//         // 这个版本的问题是，每个线程都要做这个动作，效率不高
//         while (p->next = tail_) {
//             p = p->next;
//         }
//     } while (__sync_bool_compare_and_swap(&(p->next), nullptr, node) == false);

//     __sync_bool_compare_and_swap(&tail_, p, node);
// }

/**
 *--------------------- 改进能拿到 tail 但是效率不高 v2 ---------------------
*/
template <typename T>
void LockFreeQueue<T>::push(const T& val) { // 入队
    QueueNode<T>* node = new QueueNode<T>(val); // 构造入队节点
    QueueNode<T>* p = tail_;
    
    while (true) {
        QueueNode<T>* tail = tail_;
        QueueNode<T>* next = tail->next;

        // tail_ 被修改过
        if (tail != tail_) {
            continue;
        }

        // tail_ 应该被更新但事实上没更新
        // 那我们帮他更新
        if (next != nullptr) {
            __sync_bool_compare_and_swap(&tail_, tail, next);
            // 更新可能成功可能失败（因为可能被别的线程更新了）所以需要重试
            continue;
        }

        if (__sync_bool_compare_and_swap(&(tail_->next), nullptr, node)) break;
    }

    __sync_bool_compare_and_swap(&tail_, p, node);
}

// template <typename T>
// T LockFreeQueue<T>::pop() {
//     QueueNode<T>* p = nullptr;
//     do {
//         p = head_;
//         if (p->next == nullptr) {
//             return nullptr;
//         }
//     } while(__sync_bool_compare_and_swap(&head_, p, p->next) == false);

//     T ret = std::move(p->next->value);
//     delete p;
//     return ret;
// }

template <typename T>
T LockFreeQueue<T>::pop() {
    QueueNode<T>* head = nullptr;
    QueueNode<T>* next = nullptr;
    QueueNode<T>* tail = nullptr;
    T ret;
    while (true) {
        head = head_;
        tail = tail_;
        next = head->next;

        // head 被改变
        if (head != head_) {
            continue;
        }

        // 队列为空
        if (head == tail && next == nullptr) {
            return nullptr;
        }

        // 队列之前为空，现在已经加入了新的元素
        if (head == tail && next != nullptr) {
            // 若 tail_ 还是没有加入新元素之前的值，则更新 tail_
            __sync_bool_compare_and_swap(&(tail_), tail, next);
            // 可能更新失败，因为可能被其他线程更新
            continue;
        }

        if (__sync_bool_compare_and_swap(&head_, head, next)) {
            ret = std::move(head->value);
            break;
        }
    }

    delete head;
    return ret;
}

template <typename T>
std::string LockFreeQueue<T>::toString() const {
    std::stringstream ss;
    ss << "[";
    QueueNode<T>* p = head_->next;
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

}  // namespace codebase

#endif  // CODEBASE_DS_LOCK_FREE_QUEUE

// https://coolshell.cn/articles/8239.html