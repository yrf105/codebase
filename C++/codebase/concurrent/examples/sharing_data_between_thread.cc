#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <limits.h>
#include <shared_mutex>

std::list<int> li;
std::mutex mu;

static void f(int i) {
    std::lock_guard<std::mutex> lock(mu);
    li.push_back(i);
}

static bool listContains(int x) {
    std::lock_guard<std::mutex> lock(mu);
    return std::find(li.begin(), li.end(), x) != li.end();
}

static void test() {
    std::cout << "=-=-= test =-=-=\n";

    f(5);
    f(6);
    f(7);
    f(8);
    std::cout << listContains(8) << std::endl;
}

// C++17提供了加强版的std::scoped_lock，
// 它可以接受任意数量的std::mutex，可完全取代std::lock_guard
// std::scoped_lock g(m1, m2);

class A {
private:
    int a_ = 0;

public:
    void f() {
        ++a_;
        std::cout << a_ << std::endl;
    }
};

class B {
private:
    A data_;
    std::mutex mu_;

public:
    template <typename F>
    void doSomething(F f) {
        std::scoped_lock<std::mutex> lock(mu_);
        f(data_);  // 若 f 的参数是引用或指针类型(成员函数把数据成员的指针交了出去)，加锁也没用
    }
};

A* p;
void oops(A& a) { p = &a; }

B b;
static void test1() {
    std::cout << "=-=-= test1 =-=-=\n";

    b.doSomething(oops);
    p->f();
}

class EmptyStack : public std::exception {
    const char* what() const noexcept { return "stack empty!"; }
};

// ---------------------- pop 可以返回值 stack ----------------------
template <typename T>
class Stack {
private:
    std::stack<T> st_;
    mutable std::mutex mu_;

public:
    Stack() : st_(std::stack<T>()) {}

    Stack(const Stack<T>& rhs) {
        std::lock_guard lock(rhs.mu_);
        st_ = rhs.st_;
    }

    Stack& operator=(const Stack& rhs) = delete;

    void push(T v) {
        std::lock_guard lock(mu_);
        st_.push(std::move(v));
    }

    std::shared_ptr<T> pop() {
        std::lock_guard lock(mu_);
        if (empty()) {
            throw EmptyStack();
        }
        std::shared_ptr<T> ret(std::make_shared<T>(std::move(st_.top())));
        st_.pop();
        return ret;
    }

    void pop(T& v) {
        std::lock_guard lock(mu_);
        if (empty()) {
            throw EmptyStack();
        }
        v = std::move(st_.top());
        st_.pop();
    }

    bool empty() const {
        std::lock_guard lock(mu_);
        return st_.empty();
    }
};

static void test2() {
    std::cout << "=-=-= test2 =-=-=\n";

    Stack<std::string> st;
    st.push("yue");
    std::shared_ptr<std::string> p(st.pop());
    std::cout << *p << std::endl;
}

struct C {
    explicit C(int x) : i(x) {}
    int i;
    std::mutex mu;
};

static void fooC(C& from, C& to, int n) {
    // 这里的执行逻辑是：先由 std::lock 同时获取两个锁，
    // 然后在交由 std::lock_guard 去解锁
    // 避免了死锁
    std::lock(from.mu, to.mu);

    // 下面按固定顺序加锁，看似不会有死锁的问题
    // 但如果没有std::lock同时上锁
    // 另一线程中执行f(to, from, n)
    // 两个锁的顺序就反了过来，从而可能导致死锁
    std::lock_guard lock1(from.mu, std::adopt_lock);  // std::adopt_lock表示获取mu的所有权
    std::lock_guard lock2(to.mu, std::adopt_lock);

    from.i -= n;
    to.i += n;
}

// std::lock解决了死锁问题，它可以一次性锁住多个mutex，并且没有死锁风险
static void test3() {
    std::cout << "=-=-= test3 =-=-=\n";

    C from(5);
    C to(3);

    std::thread t1(fooC, std::ref(from), std::ref(to), 5);
    std::thread t2(fooC, std::ref(to), std::ref(from), 10);

    t1.join();
    t2.join();

    std::cout << "from: " << from.i << " to: " << to.i << std::endl;
}

static void fooC2(C& from, C& to, int n) {
    std::unique_lock lock1(from.mu, std::defer_lock);  // defer_lock 表示现在不上锁，但是在出作用域会负责解锁
    std::unique_lock lock2(to.mu, std::defer_lock);
    std::lock(lock1, lock2);  // 注意这里传入的是 lock1 lock2

    from.i -= n;
    to.i += n;
}

static void test4() {
    std::cout << "=-=-= test4 =-=-=\n";

    C from(5);
    C to(3);

    std::thread t1(fooC2, std::ref(from), std::ref(to), 5);
    std::thread t2(fooC2, std::ref(to), std::ref(from), 10);

    t1.join();
    t2.join();

    std::cout << "from: " << from.i << " to: " << to.i << std::endl;
}

// unique_lock 转移锁的作用域
std::unique_lock<std::mutex> get_lock() {
    extern std::mutex mu;
    std::unique_lock<std::mutex> lock(mu);
    std::cout << "转移锁的作用域\n";
    return lock;
}

static void test5() {
    std::cout << "=-=-= test5 =-=-=\n";

    std::unique_lock<std::mutex> lock(get_lock());
    std::cout << "zy\n";
}

// 对一些费时的操作（如文件IO）上锁可能造成很多操作被阻塞，可以在面对这些操作时先解锁

// 如果支持C++17，最易最优的同时上锁方法是使用std::scoped_lock

/** 使用锁的 4 个建议
 * 1. 在一个线程里只使用一把锁 
 * 2. 按顺序上锁
 * 3. 不在上锁的程序中调用用户代码
 * 4. 使用层次锁
 */

// ---------------------------- 层次锁 ----------------------------
class HierarchyMutex {
private:
    std::mutex internal_mu_;
    // 锁的层级
    const unsigned long hierarchy_value_;
    // 线程在上这个层级锁之前上的层级锁的层级
    unsigned long previous_hierarchy_value_;
    // 线程当前上的层级锁的的层级
    static thread_local unsigned long this_thread_hierarchy_value; // 所在线程的层级值

    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value <= hierarchy_value_) {
            throw std::logic_error("hierarchy mutex violation");
        }
    }

    void update_hierarchy_value() {
        previous_hierarchy_value_ = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value_;
    }

public:
    explicit HierarchyMutex(unsigned long value) :
        hierarchy_value_(value), previous_hierarchy_value_(0) {}

    void lock() {
        check_for_hierarchy_violation();
        internal_mu_.lock();
        update_hierarchy_value();
    }

    void unlock() {
        if (this_thread_hierarchy_value != hierarchy_value_) {
            throw std::logic_error("hierarchy mutex violation");
        }
        this_thread_hierarchy_value = previous_hierarchy_value_;
        internal_mu_.unlock();
    }

    bool try_lock() {
        check_for_hierarchy_violation();
        if (!internal_mu_.try_lock()) return false;
        update_hierarchy_value();
        return true;
    }
};

// hierarchical_mutex high(10000);
// hierarchical_mutex mid(6000);
// hierarchical_mutex low(5000); // 构造一个层级锁
// // 初始化时锁的层级值hierarchy_value为5000
// // previous_hierarchy_value为0

// void lf()
// {
//     std::scoped_lock l(low);
//     // 用层级锁构造std::scoped_lock时会调用low.lock
//     // lock先检查，this_thread_hierarchy_value初始化为ULONG_MAX
//     // 因此this_thread_hierarchy_value大于hierarchy_value
//     // 通过检查，内部锁上锁
//     // 更新值，把previous_hierarchy_value更新为线程层级值ULONG_MAX
//     // 把this_thread_hierarchy_value更新为low的层级值5000
// } // 调用low.unlock，检查this_thread_hierarchy_value，值为5000
// // 与hierarchy_value相等，通过检查
// // 接着把this_thread_hierarchy_value恢复为pre保存的ULONG_MAX
// // 最后解锁

// void hf()
// {
//     std::scoped_lock l(high);
//     // this_thread_hierarchy_value更新为high的层级值10000
//     lf(); // 调用lf时，lf里的this_thread_hierarchy_value值为10000
//     // 过程只是把lf中的注释里this_thread_hierarchy_value初始值改为10000
//     // 同样能通过检查，其余过程一致，最后解锁lf会恢复到这里的线程层级值10000
// }

// void mf()
// {
//     std::scoped_lock l(mid);
//     // this_thread_hierarchy_value更新为mid的层级值6000
//     hf(); // 调用hf时，hf里的this_thread_hierarchy_value值为6000
//     // 构造hf里的l时，调用high.lock
//     // 检查this_thread_hierarchy_value，小于high.hierarchy_value
//     // 于是throw std::logic_error("mutex hierarchy violated")
// }

// 初始化为ULONG_MAX以使构造锁时能通过检查
thread_local unsigned long HierarchyMutex::this_thread_hierarchy_value(ULONG_MAX);

// std::once_flag 和 std::call_once 可以使得多线程共享的某段代码只执行一次
std::once_flag flag;

static void fooOnce() {
    std::call_once(flag, [](){ std::cout << 1; });
    std::cout << 2;
}

static void test6() {
    std::cout << "=-=-= test6 =-=-=\n";

    std::thread t1(fooOnce);
    std::thread t2(fooOnce);
    std::thread t3(fooOnce);

    t1.join();
    t2.join();
    t3.join();
}

// std::call_once 也可以用在类中
class D {
private:
    std::once_flag flag_;

private:
    void foo() {
        std::cout << 1 << std::endl;;
    }
public:
    void f() {
        std::call_once(flag_, &D::foo, this);
    }
};

// C++11规定static变量的初始化只完全发生在一个线程中，
// 直到初始化完成前其他线程都不会做处理，从而避免了race condition。
// 只有一个全局实例时可以不使用std::call_once而直接用static
// 是不是想到了单例？

class E {
private:
    mutable std::shared_mutex mu_;
    int n_ = 0;
public:
    int read() {
        std::shared_lock lock(mu_);
        return n_;
    }

    void write() {
        std::unique_lock lock(mu_);
        ++n_;
    }
};

static void test7() {
    std::cout << "=-=-= test7 =-=-=\n";

    E e;
    e.write();
    std::cout << e.read() << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "=-=-= test begin =-=-=\n";

    std::cout << "=-=-= 线程间共享数据存在的问题 =-=-=\n";

    std::cout << "=-=-= 用 mutex 保护共享数据 =-=-=\n";

    // test();

    // test1();

    // test2();

    std::cout << "=-=-= 死锁 =-=-=\n";

    // test3();

    // test4();

    // test5();

    std::cout << "=-=-= 其他保护共享数据的可选方式 =-=-=\n";
    std::cout << "=-=-= 保护共享数据的初始化 =-=-=\n";
    
    // test6();

    std::cout << "=-=-= 保护不常更新的数据结构 =-=-=\n";
    // 读多写少 读写锁 std::shared_mutex 和 std::shared_timed_mutex (C++14)
    // boost::shared_mutex (C++11)
    // 一般用std::shared_lock锁定读，std::unique_lock锁定写

    // test7();

    std::cout << "=-=-= 递归锁 =-=-=\n";
    // std::recursive_mutex，它可以在一个线程上多次获取锁，
    // 但在其他线程获取锁之前必须释放所有的锁 

    // 多数情况下，如果需要递归锁，说明代码设计存在问题。
    // 比如一个类的每个成员函数都会上锁，
    // 一个成员函数调用另一个成员函数，就可能多次上锁，
    // 这种情况用递归锁就可以避免产生未定义行为。
    // 但显然这个设计本身是有问题的，
    // 更好的办法是提取其中一个函数作为private成员并且不上锁，
    // 其他成员先上锁再调用该函数

    std::cout << "=-=-= test end =-=-=\n";
    return 0;
}