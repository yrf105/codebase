#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

struct A {
    int a[100];
};

struct B {
    int x;
    int y;
};

static void test() {
    std::cout << "=-=-= test =-=-=\n";

    std::cout << std::boolalpha << std::atomic<A>{}.is_lock_free() << std::endl // false
              << std::atomic<B>{}.is_lock_free() << std::endl; // true
}

static void test1() {
    std::cout << "=-=-= test1 =-=-=\n";

    // C++17中每个原子类型都有一个is_always_lock_free成员变量，为true时表示该原子类型在此平台上lock-free
    
    // C++17之前可以用标准库为各个原子类型定义的ATOMIC_xxx_LOCK_FREE宏来判断该类型是否无锁，值为0表示原子类型是有锁的，为2表示无锁，为1表示运行时才能确定
}

static void test2() {
    std::cout << "=-=-= test2 =-=-=\n";
    
    std::atomic_flag x = ATOMIC_FLAG_INIT;
    x.clear(std::memory_order_release); // 将状态设为clear（false）
    // 不能为读操作语义：memory_order_consume、memory_order_acquire、memory_order_acq_rel
    bool y = x.test_and_set(); // 将状态设为set（true）且返回之前的值
    std::cout << y << std::endl;
}

// ------------------ std::atomic_flag 实现自旋锁 ------------------
class spinlock_mutex {
    std::atomic_flag flag = ATOMIC_FLAG_INIT; // 注意不能在构造函数中初始化
public:
    void lock() {
        // 如果已经上锁，则返回 true，并设置 true 会死循环，直到返回 false
        while (flag.test_and_set(std::memory_order_acquire));
    }
    void unlock() {
        // 设为 false
        flag.clear(std::memory_order_release);
    }
};

spinlock_mutex m;
static void foo3(int n) {
    for (int i = 0; i < 100; ++i) {
        m.lock();

        std::cout << "-----------n: " << n << std::endl;

        m.unlock();
    }
}

static void test3() {
    std::cout << "=-=-= test3 =-=-=\n";

    std::vector<std::thread> ths;
    for (int i = 0; i < 10; ++i) {
        ths.emplace_back(foo3, i);
    }
    for (auto& t : ths) {
        t.join();
    }
}

// typedef enum memory_order {
//     memory_order_relaxed, // 无同步或顺序限制，只保证当前操作原子性
//     memory_order_consume, // 标记读操作，依赖于该值的读写不能重排到此操作前
//     memory_order_acquire, // 标记读操作，之后的读写不能重排到此操作前
//     memory_order_release, // 标记写操作，之前的读写不能重排到此操作后
//     memory_order_acq_rel, // 仅标记读改写操作，读操作相当于acquire，写操作相当于release
//     memory_order_seq_cst // sequential consistency：顺序一致性，不允许重排，所有原子操作的默认选项
// } memory_order;

static void test4() {
    std::cout << "=-=-= test4 =-=-=\n";
    
    std::atomic<bool> x(true);
    x = false;
    // bool y = x.load(std::memory_order_acquire);
    x.store(true);

    // y = x.exchange(false, std::memory_order_acq_rel);

    std::cout << x.is_lock_free() << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "=-=-= test begin =-=-=\n";

    std::cout << "=-=-= 内存模型基础 =-=-=\n";
    /*
    为了避免race
    condition，线程就要规定执行顺序。一种方式是使用mutex，后一线程必须等待前一线程解锁。第二种方式是使用原子操作来避免竞争访问同一内存位置
    原子操作是不可分割的操作，要么做了要么没做，不存在做一半的状态。如果读取对象值的加载操作是原子的，那么对象上的所有修改操作也是原子的，读取的要么是初始值，要么是某个修改完成后的存储值。因此，原子操作不存在修改过程中值被其他线程看到的情况，也就避免了竞争风险
    每个对象从初始化开始都有一个修改顺序，这个顺序由来自所有线程对该对象的写操作组成。通常这个顺序在运行时会变动，但在任何给定的程序执行中，系统中所有线程都必须遵循此顺序
    如果对象不是原子类型，就要通过同步来保证线程遵循每个变量的修改顺序。如果一个变量对于不同线程表现出不同的值序列，就会导致数据竞争和未定义行为。使用原子操作就可以把同步的责任抛给编译器
    */
    std::cout << "=-=-= 原子操作和原子类型 =-=-=\n";
    std::cout << "=-=-= 标准原子类型 =-=-=\n";
    // test();

    // test1();

    std::cout << "=-=-= std::atomic_flag=-=-=\n";
    
    // test3();

    std::cout << "=-=-= 其他原子类型 =-=-=\n";
    test4();

    std::cout << "=-=-= test end =-=-=\n";

    return 0;
}