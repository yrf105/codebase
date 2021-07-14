#include <iostream>
#include <memory>

class Beta;
struct Alpha{
    using ptr = std::shared_ptr<Alpha>;

    ~Alpha() {
        std::cout << "Alpha 析构了\n";
    }

    // weak_ptr 指向一个对象但是不增加引用计数
    std::weak_ptr<Beta> beta_;
};

struct Beta{
    using ptr = std::shared_ptr<Beta>;

    ~Beta() {
        std::cout << "Belta 析构了\n";
    }

    Alpha::ptr alpha_;
};

int main() {
    Alpha::ptr a(new Alpha);
    Beta::ptr b(new Beta);
    
    // Belta 析构了
    // Alpha 析构了
    a->beta_ = b;
    Beta::ptr bb = a->beta_.lock(); // 通过 lock 判断 weak_ptr 管理的资源是否被释放
    if (bb) {
        // ...
    }
    b->alpha_ = a;

    return 0;
}