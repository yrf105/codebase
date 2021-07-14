#include <iostream>
#include <set>
#include <string>
#include <iomanip>

// class A {
// public:
//     A() : str_(){};  // public 的构造函数
//     // explicit A(const std::string& str) : str_(str) {}

//     void destroy() {  // 伪 destructor
//         delete this;
//     }

// protected:
//     ~A() {}  // private 的析构函数

// private:
//     std::string str_;
// };

// class AA : public A {};

// class Alpha {
//     A* a_;
// };

class HeapTracked {
public:
    class MissingAddress {};

    virtual ~HeapTracked() = 0;

    static void* operator new(size_t size);
    static void operator delete(void* ptr);

    bool isOnHeap() const;

private:
    using RawAddress = const void*;
    static std::set<RawAddress> addresses_;
};

std::set<HeapTracked::RawAddress> HeapTracked::addresses_{};

HeapTracked::~HeapTracked() {}

void* HeapTracked::operator new(size_t size) {
    void* ptr = ::operator new(size);
    addresses_.insert(ptr);
    return ptr;
}

void HeapTracked::operator delete(void* ptr) {
    if (addresses_.find(ptr) == addresses_.end()) {
        throw MissingAddress();
    }

    ::operator delete(ptr);
}

bool HeapTracked::isOnHeap() const {
    // 涉及多继承或虚拟继承的对象会有多个地址，通过 dynamic_cast 消除该问题
    // dynamic_cast 只适用于「所指对象至少有一个虚函数」的指针身上
    RawAddress rawAddress = dynamic_cast<RawAddress>(this);
    return addresses_.find(rawAddress) != addresses_.end();
}

class A : public HeapTracked {
public:

private:
    std::string str_;
};

int main() {
    // // error: "A::~A() noexcept" (declared at line 14) is
    // inaccessibleC/C++(330) A a;

    // A* a = new A;
    // a->destroy();

    // // error: the default constructor of "AA" cannot be referenced -- it is a
    // // deleted function
    // AA aa;

    // Alpha alpha;


    A a;
    A* p = &a;
    std::cout << std::boolalpha << p->isOnHeap() << std::endl; // fasle

    A* ph = new A;
    std::cout << std::boolalpha << ph->isOnHeap() << std::endl; // true

    return 0;
}