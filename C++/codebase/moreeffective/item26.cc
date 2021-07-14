#include <iostream>
#include <memory>

template <typename BeingCounted>
class Counted {
public:
    class TooManyObjs {};
    static int objCount() { return numObjs_; }

protected:
    Counted();
    Counted(const Counted&);
    ~Counted() { --numObjs_; }

private:
    void init();

private:
    static size_t numObjs_;
    const static size_t maxNumObjs_;
};

// maxNumObjs_ 的初始化交由客户端实现
// 应当放在 .c 文件里
template <typename BeingCounted>
size_t Counted<BeingCounted>::numObjs_ = 0;

template <typename BeingCounted>
Counted<BeingCounted>::Counted() {
    init();
}

template <typename BeingCounted>
Counted<BeingCounted>::Counted(const Counted<BeingCounted>&) {
    init();
}

template <typename BeingCounted>
void Counted<BeingCounted>::init() {
    if (numObjs_ >= maxNumObjs_) {
        throw TooManyObjs();
    }
    ++numObjs_;
}

class A : private Counted<A> {  // private 继承
public:
    using Counted<A>::objCount;  // using 使成为 public
    using Counted<A>::TooManyObjs;

    static A* makeA() { return new A; }

    static A* makeA(const A& rhs) { return new A(rhs); }

    void setStr(const std::string& str) { str_ = str; }

    std::string str() const { return str_; }

private:
    A() : str_() {}
    A(const A& rhs) : str_(rhs.str_) {}

private:
    std::string str_;
};

// 客户端负责初始化 maxNumObjs_
template <>
const size_t Counted<A>::maxNumObjs_ = 1;

int main() {
    // std::unique_ptr<A> a(A::makeA());
    // //terminate called after throwing an instance of
    // 'Counted<A>::TooManyObjs'
    // // [1]    160082 abort (core dumped)  ../bin/item26
    // std::unique_ptr<A> a2(A::makeA());

    return 0;
}