#include "design_patterns/singleton.h"

#include <iostream>
#include <string>
#include <thread>


class A {
public:
    const std::string& name() const { return name_; }
    void set_name(const std::string& v) { name_ = v; }

private:
    // A() = delete;
    // A(const A&) = delete;
    // A(const A&&) = delete;
    // A& operator=(const A&) = delete;

private:
    std::string name_;
};

void foo1() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::shared_ptr<A> a1 = codebase::SingletonPtr<A>::GetInstancePtr();
    a1->set_name("a1");
    std::cout << "foo1 " << a1->name() << std::endl;
}

void foo2() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::shared_ptr<A> a2 = codebase::SingletonPtr<A>::GetInstancePtr();
    std::cout << "foo2 " << a2->name() << std::endl;
}

void test2() {
    std::cout << "多线程智能指针\n";
    std::thread t1(foo1);
    std::thread t2(foo2);
    t1.join();
    t2.join();
}

void foo3() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    A* a3 = codebase::Singleton<A>::GetInstance();
    a3->set_name("a3");
    std::cout << "foo3 " << a3->name() << std::endl;
}

void foo4() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    A* a4 = codebase::Singleton<A>::GetInstance();
    std::cout << "foo4 " << a4->name() << std::endl;
}

void test1() {
    std::cout << "多线程裸指针\n";
    std::thread t1(foo3);
    std::thread t2(foo4);

    t1.join();
    t2.join();
}

void test() {
    std::cout << "单线程裸指针\n";
    A* a1 = codebase::Singleton<A>::GetInstance();
    a1->set_name("yue");
    std::cout << "test " <<  a1->name() << std::endl;
    A* a2 = codebase::Singleton<A>::GetInstance();
    std::cout << "test " << a2->name() << std::endl;
}

void foo5() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    A* a5 = codebase::SingletonDCLP<A>::GetInstance();
    a5->set_name("a5");
    std::cout << "foo5 " << a5->name() << std::endl;
}

void foo6() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    A* a6 = codebase::SingletonDCLP<A>::GetInstance();
    std::cout << "foo6 " << a6->name() << std::endl;
}

void test3() {
    std::cout << "双重检查锁模式 DCLP\n";
    std::thread t1(foo5);
    std::thread t2(foo6);

    t1.join();
    t2.join();
}

int main(int argc, char** argv) {
    test();
    test1();
    test2();
    test3();
    return 0;
}

// https://www.bookstack.cn/read/CPlusPlusThings/68fdd6c8536795e6.md#8gjy7m