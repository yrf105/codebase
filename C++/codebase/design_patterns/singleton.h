#ifndef CODEBASE_DESIGN_PATTERNS_SINGLETON_H_
#define CODEBASE_DESIGN_PATTERNS_SINGLETON_H_

#include <memory>
#include <thread>
#include <mutex>

namespace codebase {

template <typename T>
class Singleton {
public:
    static T* GetInstance() {
        static T t;
        return &t;
    }
};

template <typename T>
class SingletonPtr {
public:
    static std::shared_ptr<T> GetInstancePtr() {
        static std::shared_ptr<T> t = std::make_shared<T>();
        return t;
    }
};

template <typename T>
class SingletonDCLP {
public:
    static T* GetInstance() {
        if (t_ == nullptr) {
            std::lock_guard<std::mutex> lock(mu_);
            if (t_ == nullptr) {
                t_ = new T();
                return t_;
            }
        }

        return t_;
    }

private:
    static T* t_;
    static std::mutex mu_;
};

template <typename T>
T* SingletonDCLP<T>::t_ = nullptr;

template <typename T>
std::mutex SingletonDCLP<T>::mu_;

} // namespace CODEBASE_DESIGN_PATTERNS_SINGLETON_H_

#endif // CODEBASE_DESIGN_PATTERNS_SINGLETON_H_

// https://www.bookstack.cn/read/CPlusPlusThings/68fdd6c8536795e6.md#8gjy7m