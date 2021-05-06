#include <iostream>

using func = void(*)(void);

static void foo1() {
    double a = 1;
    double b = 2;
    std::cout << &a << " " << &b << " foo1\n";
}

class Base{
public:
    virtual void foo1() const { std::cout << "Base::foo1\n"; }
    virtual void foo2() const { std::cout << "Base::foo2\n"; }
    virtual void foo3() const { std::cout << "Base::foo3\n"; }
};

class Base1{
public:
    virtual void foo1() const { std::cout << "Base1::foo1\n"; }
    virtual void foo2() const { std::cout << "Base1::foo2\n"; }
};

class Base2{
public:
    virtual void foo1() const { std::cout << "Base2::foo1\n"; }
    virtual void foo2() const { std::cout << "Base2::foo2\n"; }
    virtual void foo3() const { std::cout << "Base2::foo3\n"; }
    virtual void foo4() const { std::cout << "Base2::foo4\n"; }
};

class Derive : public Base {
public:
    virtual void foo4() const { std::cout << "Derive::foo4\n"; }
    void foo5() const { std::cout << "Derive::foo5\n"; }
};

class Derive2 : public Base {
public:
    virtual void foo1() const override { std::cout << "Derive::foo1\n"; }
    virtual void foo4() const { std::cout << "Derive::foo4\n"; }
    void foo5() const { std::cout << "Derive::foo5\n"; }
};

class Dreive3 : public Base, public Base1, public Base2 {
public:
    virtual void foo5() const { std::cout << "Derive::foo5\n"; }
    void foo6() const { std::cout << "Derive::foo6\n"; }
};

class Dreive4 : public Base, public Base1, public Base2 {
public:
    virtual void foo1() const override { std::cout << "Derive::foo1\n"; }
    virtual void foo5() const { std::cout << "Derive::foo5\n"; }

    void foo6() const { std::cout << "Derive::foo6\n"; }
};

namespace test11 {

class Animal{
public:
    virtual void eat() { std::cout << "Animal::eat\n"; }
};

class Mammal : public virtual Animal{
public:
    virtual void breath() { std::cout << "Mammal::breath\n"; }
};

class WingedAnimal : public virtual Animal{
public:
    virtual void fly() { std::cout << "WingedAnimal::fly\n"; }
    virtual void flap() { std::cout << "WingedAnimal::flap\n"; }
};

class Bat : public Mammal, public WingedAnimal {
public:
    virtual void Sonar() { std::cout << "Bat::Sonar\n"; }
};

static void test11() {
    std::cout << "-------------> 多重继承（虚继承解决钻石问题）（无虚函数覆盖）\n";

    // vtable for Bat:
    //     .quad   0
    //     .quad   0
    //     .quad   0
    //     .quad   typeinfo for Bat
    //     .quad   Animal::eat()
    //     .quad   Mammal::breath()
    //     .quad   Bat::Sonar()
    //     .quad   -8
    //     .quad   -8
    //     .quad   -8
    //     .quad   typeinfo for Bat
    //     .quad   0
    //     .quad   WingedAnimal::fly()

    Bat bat1;

    std::cout << "bat1 实例地址：" << (uintptr_t*)(&bat1) << std::endl;
    std::cout << "bat1 Mammal 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&bat1) << std::endl;
    std::cout << "bat1 Mammal 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&bat1) << std::endl;

    func pFunc1_1 = (func)*(uintptr_t*)*(uintptr_t*)(&bat1);
    func pFunc1_2 = (func)*((uintptr_t*)*(uintptr_t*)(&bat1) + 1);
    func pFunc1_3 = (func)*((uintptr_t*)*(uintptr_t*)(&bat1) + 2);


    pFunc1_1();
    pFunc1_2();
    pFunc1_3();

    std::cout << "bat1 Mammal 虚函数表结束地址：" << ((uintptr_t*)*(uintptr_t*)(&bat1) + 4)
            << " 值为：" << (int)*((uintptr_t*)*(uintptr_t*)(&bat1) + 4)
            << " " << (uintptr_t*)*((uintptr_t*)*(uintptr_t*)(&bat1) + 4) << std::endl;

    std::cout << "bat1 WingedAnimal 虚函数表地址：" << (uintptr_t*)*((uintptr_t*)(&bat1) + 1) << std::endl;
    std::cout << "bat1 WingedAnimal 虚函数表中第一个函数的地址：" << (uintptr_t*)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 1) << std::endl;
    // 虚继承后只有第一个直接父类，存有超父类的虚函数指针
    // 
    // func pFunc2_1 = (func)*(uintptr_t*)*((uintptr_t*)(&bat1) + 1);
    func pFunc2_2 = (func)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 1); 
    func pFunc2_3 = (func)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 2); // 只有第一个虚函数表里有派生类特有的虚函数


    // pFunc2_1();
    pFunc2_2();
    pFunc2_3();

    std::cout << "bat1 WingedAnimal 虚函数表结束地址：" << ((uintptr_t*)*((uintptr_t*)(&bat1) + 1))
            << " 值为：" << (int)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1))
            << " " << (uintptr_t*)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1)) << std::endl;

}
} // end of namespace test11

class Animal{
public:
    virtual void eat() { std::cout << "Animal::eat\n"; }
};

class Mammal : public Animal{
public:
    virtual void breath() { std::cout << "Mammal::breath\n"; }
};

class WingedAnimal : public Animal{
public:
    virtual void fly() { std::cout << "WingedAnimal::fly\n"; }
};

class Bat : public Mammal, public WingedAnimal {
public:
    virtual void Sonar() { std::cout << "Bat::Sonar\n"; }
};

static void test10() {
    std::cout << "-------------> 多重继承（存在钻石问题）（无虚函数覆盖）\n";

    // vtable for Bat:
    //     .quad   0
    //     .quad   typeinfo for Bat
    //     .quad   Animal::eat()
    //     .quad   Mammal::breath()
    //     .quad   Bat::Sonar()
    //     .quad   -8
    //     .quad   typeinfo for Bat
    //     .quad   Animal::eat()
    //     .quad   WingedAnimal::fly()

    Bat bat1;

    std::cout << "bat1 实例地址：" << (uintptr_t*)(&bat1) << std::endl;
    std::cout << "bat1 Mammal 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&bat1) << std::endl;
    std::cout << "bat1 Mammal 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&bat1) << std::endl;

    func pFunc1_1 = (func)*(uintptr_t*)*(uintptr_t*)(&bat1);
    func pFunc1_2 = (func)*((uintptr_t*)*(uintptr_t*)(&bat1) + 1);
    func pFunc1_3 = (func)*((uintptr_t*)*(uintptr_t*)(&bat1) + 2);


    pFunc1_1();
    pFunc1_2();
    pFunc1_3();

    std::cout << "bat1 Mammal 虚函数表结束地址：" << ((uintptr_t*)*(uintptr_t*)(&bat1) + 3)
            << " 值为：" << (int)*((uintptr_t*)*(uintptr_t*)(&bat1) + 3)
            << " " << (uintptr_t*)*((uintptr_t*)*(uintptr_t*)(&bat1) + 3) << std::endl;

    std::cout << "bat1 WingedAnimal 虚函数表地址：" << (uintptr_t*)*((uintptr_t*)(&bat1) + 1) << std::endl;
    std::cout << "bat1 WingedAnimal 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*((uintptr_t*)(&bat1) + 1)<< std::endl;

    func pFunc2_1 = (func)*(uintptr_t*)*((uintptr_t*)(&bat1) + 1);
    func pFunc2_2 = (func)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 1);
    // func pFunc2_3 = (func)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 2); // 只有第一个虚函数表里有派生类特有的虚函数


    pFunc2_1();
    pFunc2_2();
    // pFunc2_3();

    std::cout << "bat1 WingedAnimal 虚函数表结束地址：" << ((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 2)
            << " 值为：" << (int)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 2)
            << " " << (uintptr_t*)*((uintptr_t*)*((uintptr_t*)(&bat1) + 1) + 2) << std::endl;
}

class Person2 : public Mammal{
public:
    virtual void eat() override { std::cout << "Person::eat\n"; }
    virtual void breath() override { std::cout << "Person::breath\n"; }

    virtual void think() { std::cout << "Person::think\n"; }
};

static void test9() {
    std::cout << "-------------> 多重继承（有虚函数覆盖）\n";

    Person2 p2;

    std::cout << "p2 实例地址：" << (uintptr_t*)(&p2) << std::endl;
    std::cout << "p2 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&p2) << std::endl;
    std::cout << "p2 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&p2) << std::endl;

    func pFunc2_1 = (func)*(uintptr_t*)*(uintptr_t*)(&p2);
    func pFunc2_2 = (func)*((uintptr_t*)*(uintptr_t*)(&p2) + 1);
    func pFunc2_3 = (func)*((uintptr_t*)*(uintptr_t*)(&p2) + 2);
    
    pFunc2_1();
    pFunc2_2();
    pFunc2_3();

    std::cout << "p2 虚函数表结束地址：" << ((uintptr_t*)*(uintptr_t*)(&p2) + 3)
              << " 值为：" << (int)*((uintptr_t*)*(uintptr_t*)(&p2) + 3)
              << " " << (uintptr_t*)*((uintptr_t*)*(uintptr_t*)(&p2) + 3) << std::endl;
}

class Person : public Mammal{
public:
    virtual void think() { std::cout << "Person::think\n"; }
};

static void test8() {
    std::cout << "-------------> 多重继承（无虚函数覆盖）\n";

    Person p1;

    std::cout << "p1 实例地址：" << (uintptr_t*)(&p1) << std::endl;
    std::cout << "p1 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&p1) << std::endl;
    std::cout << "p1 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&p1) << std::endl;

    func pFunc1_1 = (func)*(uintptr_t*)*(uintptr_t*)(&p1);
    func pFunc1_2 = (func)*((uintptr_t*)*(uintptr_t*)(&p1) + 1);
    func pFunc1_3 = (func)*((uintptr_t*)*(uintptr_t*)(&p1) + 2);
    
    pFunc1_1();
    pFunc1_2();
    pFunc1_3();

    std::cout << "p1 虚函数表结束地址：" << ((uintptr_t*)*(uintptr_t*)(&p1) + 3)
              << " 值为：" << (int)*((uintptr_t*)*(uintptr_t*)(&p1) + 3)
              << " " << (uintptr_t*)*((uintptr_t*)*(uintptr_t*)(&p1) + 3) << std::endl;
}

static void test7() {
    std::cout << "-------------> 多继承（有虚函数覆盖）\n";
    
    Dreive4 d4_1;

    std::cout << "d4_1 实例地址：" << (uintptr_t*)(&d4_1) << std::endl;

    std::cout << "d4_1 base 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&d4_1) << std::endl;
    std::cout << "d4_1 base 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&d4_1) << std::endl;

    func pFunc1 = (func)*(uintptr_t*)*(uintptr_t*)(&d4_1);
    func pFunc2 = (func)*((uintptr_t*)*(uintptr_t*)(&d4_1) + 1);
    func pFunc3 = (func)*((uintptr_t*)*(uintptr_t*)(&d4_1) + 2);
    func pFunc4 = (func)*((uintptr_t*)*(uintptr_t*)(&d4_1) + 3);

    pFunc1();
    pFunc2();
    pFunc3();
    pFunc4();

    std::cout << "d4_1 base 虚函数表结束地址：" << ((uintptr_t*)*(uintptr_t*)(&d4_1) + 4)
              << " 值为：" << (int)*((uintptr_t*)*(uintptr_t*)(&d4_1) + 4)
              << " " << (uintptr_t*)*((uintptr_t*)*(uintptr_t*)(&d4_1) + 4) << std::endl;

    std::cout << "d4_1 base1 虚函数表地址：" << (uintptr_t*)*((uintptr_t*)(&d4_1) + 1) << std::endl;
    std::cout << "d4_1 base1 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*((uintptr_t*)(&d4_1) + 1) << std::endl;

    func pFunc1_1 = (func)*(uintptr_t*)*((uintptr_t*)(&d4_1) + 1);
    func pFunc1_2 = (func)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 1) + 1);
    // func pFunc1_3 = (func)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 1) + 2); // 只有第一个虚函数表里有派生类特有的虚函数

    pFunc1_1();
    pFunc1_2();

    std::cout << "d4_1 base1 虚函数表结束地址：" << ((uintptr_t*)*((uintptr_t*)(&d4_1) + 1) + 2)
              << " 值为：" << (int)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 1) + 2)
              << " " << (uintptr_t*)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 1) + 2) << std::endl;


    std::cout << "d4_1 base2 虚函数表地址：" << (uintptr_t*)*((uintptr_t*)(&d4_1) + 2) << std::endl;
    std::cout << "d4_1 base2 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*((uintptr_t*)(&d4_1) + 2) << std::endl;

    func pFunc2_1 = (func)*(uintptr_t*)*((uintptr_t*)(&d4_1) + 2);
    func pFunc2_2 = (func)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 2) + 1);
    func pFunc2_3 = (func)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 2) + 2);
    func pFunc2_4 = (func)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 2) + 3);
    // func pFunc1_3 = (func)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 1) + 2); // 只有第一个虚函数表里有派生类特有的虚函数

    pFunc2_1();
    pFunc2_2();
    pFunc2_3();
    pFunc2_4();

    std::cout << "d4_1 base2 虚函数表结束地址：" << ((uintptr_t*)*((uintptr_t*)(&d4_1) + 2) + 4)
              << " 值为：" << (int)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 2) + 4)
              << " " << (uintptr_t*)*((uintptr_t*)*((uintptr_t*)(&d4_1) + 2) + 4) << std::endl;
}

static void test6() {
    std::cout << "-------------> 多继承（无虚函数覆盖）\n";
    
    Dreive3 d3_1;

    std::cout << "d3_1 实例地址：" << (uintptr_t*)(&d3_1) << std::endl;

    std::cout << "d3_1 base 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&d3_1) << std::endl;
    std::cout << "d3_1 base 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&d3_1) << std::endl;

    func pFunc1 = (func)*(uintptr_t*)*(uintptr_t*)(&d3_1);
    func pFunc2 = (func)*((uintptr_t*)*(uintptr_t*)(&d3_1) + 1);
    func pFunc3 = (func)*((uintptr_t*)*(uintptr_t*)(&d3_1) + 2);
    func pFunc4 = (func)*((uintptr_t*)*(uintptr_t*)(&d3_1) + 3);

    pFunc1();
    pFunc2();
    pFunc3();
    pFunc4();

    std::cout << "d3_1 base 虚函数表结束地址：" << ((uintptr_t*)*(uintptr_t*)(&d3_1) + 4)
              << " 值为：" << (int)*((uintptr_t*)*(uintptr_t*)(&d3_1) + 4)
              << " " << (uintptr_t*)*((uintptr_t*)*(uintptr_t*)(&d3_1) + 4) << std::endl;

    std::cout << "d3_1 base1 虚函数表地址：" << (uintptr_t*)*((uintptr_t*)(&d3_1) + 1) << std::endl;
    std::cout << "d3_1 base1 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*((uintptr_t*)(&d3_1) + 1) << std::endl;

    func pFunc1_1 = (func)*(uintptr_t*)*((uintptr_t*)(&d3_1) + 1);
    func pFunc1_2 = (func)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 1) + 1);
    // func pFunc1_3 = (func)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 1) + 2); // 只有第一个虚函数表里有派生类特有的虚函数

    pFunc1_1();
    pFunc1_2();

    std::cout << "d3_1 base1 虚函数表结束地址：" << ((uintptr_t*)*((uintptr_t*)(&d3_1) + 1) + 2)
              << " 值为：" << (int)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 1) + 2)
              << " " << (uintptr_t*)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 1) + 2) << std::endl;


    std::cout << "d3_1 base2 虚函数表地址：" << (uintptr_t*)*((uintptr_t*)(&d3_1) + 2) << std::endl;
    std::cout << "d3_1 base2 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*((uintptr_t*)(&d3_1) + 2) << std::endl;

    func pFunc2_1 = (func)*(uintptr_t*)*((uintptr_t*)(&d3_1) + 2);
    func pFunc2_2 = (func)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 2) + 1);
    func pFunc2_3 = (func)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 2) + 2);
    func pFunc2_4 = (func)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 2) + 3);
    // func pFunc1_3 = (func)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 1) + 2); // 只有第一个虚函数表里有派生类特有的虚函数

    pFunc2_1();
    pFunc2_2();
    pFunc2_3();
    pFunc2_4();

    std::cout << "d3_1 base2 虚函数表结束地址：" << ((uintptr_t*)*((uintptr_t*)(&d3_1) + 2) + 4)
              << " 值为：" << (int)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 2) + 4)
              << " " << (uintptr_t*)*((uintptr_t*)*((uintptr_t*)(&d3_1) + 2) + 4) << std::endl;
}

static void test5() {
    Derive2 d2_3;
    Derive2 d2_4;
    Derive2 d2_5;
    int i = 1;
    
    std::cout << "i 的地址：" << &i << std::endl;
    std::cout << "d2_3 实例地址：" << (uintptr_t*)(&d2_3) << std::endl;
    std::cout << "d2_4 实例地址：" << (uintptr_t*)(&d2_4) << std::endl;
    std::cout << "d2_5 实例地址：" << (uintptr_t*)(&d2_5) << std::endl;
}

static void test4() {
    Derive d1_3;
    Derive d1_4;
    
    std::cout << "d1_3 实例地址：" << (uintptr_t*)(&d1_3) << std::endl;
    std::cout << "d1_4 实例地址：" << (uintptr_t*)(&d1_4) << std::endl;
}

// 一般继承（有虚函数覆盖）
static void test3() {
    std::cout << "-------------> 一般继承（有虚函数覆盖）\n";

    Derive2 d2_1;
    Derive2 d2_2;

    std::cout << "d2_1 实例地址：" << (uintptr_t*)(&d2_1) << std::endl;
    std::cout << "d2_1 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&d2_1) << std::endl;
    std::cout << "d2_1 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&d2_1) << std::endl;

    std::cout << "d2_2 实例地址：" << (uintptr_t*)(&d2_2) << std::endl;
    std::cout << "d2_2 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&d2_2) << std::endl;
    std::cout << "d2_2 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&d2_2) << std::endl;

    func foo1 = (func)*((uintptr_t*)*(uintptr_t*)(&d2_1));
    func foo2 = (func)*((uintptr_t*)*(uintptr_t*)(&d2_1) + 1);
    func foo3 = (func)*((uintptr_t*)*(uintptr_t*)(&d2_1) + 2);
    func foo4 = (func)*((uintptr_t*)*(uintptr_t*)(&d2_1) + 3);

    foo1();
    foo2();
    foo3();
    foo4();

    std::cout << (int)*((uintptr_t*)*(uintptr_t*)(&d2_1) + 4) << std::endl;
}

// 一般继承（无虚函数覆盖）
static void test2() {
    std::cout << "-------------> 一般继承（无虚函数覆盖）\n";

    Derive d1_1;
    Derive d1_2;

    std::cout << "d1_1 实例地址：" << (uintptr_t*)(&d1_1) << std::endl;
    std::cout << "d1_1 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&d1_1) << std::endl;
    std::cout << "d1_1 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&d1_1) << std::endl;

    std::cout << "d1_2 实例地址：" << (uintptr_t*)(&d1_2) << std::endl;
    std::cout << "d1_2 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&d1_2) << std::endl;
    std::cout << "d1_2 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&d1_2) << std::endl;

    func foo1 = (func)*((uintptr_t*)*(uintptr_t*)(&d1_1));
    func foo2 = (func)*((uintptr_t*)*(uintptr_t*)(&d1_1) + 1);
    func foo3 = (func)*((uintptr_t*)*(uintptr_t*)(&d1_1) + 2);
    func foo4 = (func)*((uintptr_t*)*(uintptr_t*)(&d1_1) + 3);

    foo1();
    foo2();
    foo3();
    foo4();

    std::cout << (int)*((uintptr_t*)*(uintptr_t*)(&d1_2) + 4) << std::endl;
}

// 1 虚函数表指针位于对象实例最前面
static void test1() {
    Base b;

    func pFunc1 = nullptr;
    func pFunc2 = nullptr;
    func pFunc3 = nullptr;

    std::cout << "b 实例地址：" << (uintptr_t*)(&b) << std::endl;
    std::cout << "b 虚函数表地址：" << (uintptr_t*)*(uintptr_t*)(&b) << std::endl;
    std::cout << "b 虚函数表中第一个函数的地址：" << (uintptr_t*)*(uintptr_t*)*(uintptr_t*)(&b) << std::endl;

    pFunc1 = (func)*((uintptr_t*)*(uintptr_t*)(&b));
    pFunc2 = (func)*((uintptr_t*)*(uintptr_t*)(&b) + 1);
    pFunc3 = (func)*((uintptr_t*)*(uintptr_t*)(&b) + 2);

    pFunc1();
    pFunc2();
    pFunc3();
}

int main(int argc, char** argv) {

    // test1();

    // test2();

    // test3();

    // std::cout << "-------------> “栈复用”\n";
    // test4();
    // foo1();
    // test5();

    // test6();

    // test7();

    // test8();

    // test9();

    test10();

    test11::test11();

    return 0;
}

// https://coolshell.cn/articles/12165.html#%E9%99%84%E5%BD%95%E4%B8%80%EF%BC%9AVC%E4%B8%AD%E6%9F%A5%E7%9C%8B%E8%99%9A%E5%87%BD%E6%95%B0%E8%A1%A8