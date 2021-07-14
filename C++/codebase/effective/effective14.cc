#include <iostream>
#include <pthread.h>
#include <memory>
#include <boost/noncopyable.hpp>

// 引用计数
class LockMutex {
public:
    // RAII 并指定删除器 - shared_ptr 引用计数减为零时的动作
    explicit LockMutex(pthread_mutex_t* mutex) : mutexPtr_(mutex, [](pthread_mutex_t* mutex){ pthread_mutex_unlock(mutex); }) {
        pthread_mutex_lock(mutexPtr_.get());
    }
    // 使用默认析构函数，析构函数会自动调用非静态成员的析构函数，mutexPtr_ 的析构函数在引用计数减为 0 时自动调用之前指定的删除器

private:
    std::shared_ptr<pthread_mutex_t> mutexPtr_;
};

pthread_mutex_t g_mu; // 定义互斥量
int main() {
    { // 建立临界区
        LockMutex lock(&g_mu); // 上锁
        // ...
    } // 自动解锁
}