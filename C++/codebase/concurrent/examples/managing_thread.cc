#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>

static void f() { std::cout << "hello, world\n"; }

static void test() {
    std::cout << "=-=-= test =-=-=\n";
    std::thread t1(f);
    t1.join();
}

struct A {
    void operator()() const { std::cout << "仿函数\n"; }
};

static void test2() {
    std::cout << "=-=-= test2 =-=-=\n";

    A a;
    std::thread t(a);
    // expression must have class type but it has type "std::thread (*)(A (*)())"
    // std::thread t1(A());
    std::thread t2{A()};
    std::thread t3([]() { std::cout << "lambda\n"; });

    t.join();
    // t1.join();
    t2.join();
    t3.join();
}

struct B {
    int& x_;
    B(int& x) : x_(x) {}
    void operator()() const {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << x_ << std::endl;
    }
};

static void test3() {
    std::cout << "=-=-= test3 =-=-=\n";

    int x = 5;
    B b(x);
    std::thread t(b);
    // detach 会导致悬空引用，改为 join 可保证局部变量在线程结束后销毁
    t.detach();
}

static void test4() {
    std::cout << "=-=-= test4 =-=-=\n";
    std::thread t(f);
    t.join();
    // t.join(); // join 过的线程不能再 join
}

static void test5() {
    std::cout << "=-=-= test5 =-=-=\n";

    // 如果线程运行过程中发生异常，之后调用的join会被忽略，
    // 为此需要捕获异常并在处理异常时调用join

    std::thread t(f);
    try {
        std::string(0);
    } catch (...) {
        std::cout << "t.join();" << std::endl;
        t.join();
        throw;
    }
    t.join();
}

/**
 * 使用 RAII 手法管理线程
 */
class ThreadGuard {
public:
    explicit ThreadGuard(std::thread& t) : t_(t) {}
    ~ThreadGuard() {
        if (t_.joinable()) {
            std::cout << "t_.join()\n";
            t_.join();
        }
    }

private:
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;

private:
    std::thread& t_;  // 这里是引用
};

static void test6() {
    std::cout << "=-=-= test6 =-=-=\n";

    std::thread t(f);
    ::sleep(2);
    ThreadGuard tg(t);  // 调用这句时虽然 t 可能已经执行完毕了，但仍然有用
}

static void test7() {
    std::cout << "=-=-= test7 =-=-=\n";

    std::thread t(f);
    t.detach();
    // detach 后的线程不能 join
    std::cout << t.joinable() << std::endl;
}

/**
 * 分离线程称为守护线程，即没有任何显式接口并运行在后台的线程，
 * 其特点是长时间运行。比如有一个文档处理应用，为了同时编辑多个文档，
 * 每次打开一个新文档则可以开一个分离线程
 */

static void f2(int i = 1) { std::cout << i << std::endl; }

static void test8() {
    std::cout << "=-=-= test8 =-=-=\n";

    // 第一个参数为函数名，其余参数为函数的参数
    std::thread t(f2, 8);

    t.join();
}

static void f3(int& n) { n += n; }

static void test9() {
    std::cout << "=-=-= test9 =-=-=\n";

    int n = 5;
    // 这么写会报错
    // std::thread t(f3, n);
    // t.join();

    // std::cout << "n=" << n << std::endl;

    // std::thread会无视参数的引用类型，
    // 因此需要使用std::ref来生成一个引用包裹对象以传入引用类型
    std::thread t1(f3, std::ref(n));
    t1.join();

    std::cout << "n=" << n << std::endl;
}

struct C {
    void f(int& n) { n += n; }
};

static void test10() {
    std::cout << "=-=-= test10 =-=-=\n";

    // 传递成员函数
    C c;
    int n = 5;
    std::thread t(&C::f, &c, std::ref(n));
    t.join();
    std::cout << "n=" << n << std::endl;
}

static void f4(std::unique_ptr<int> p) { std::cout << *p << std::endl; }

static void test11() {
    std::cout << "=-=-= test11 =-=-=\n";

    std::unique_ptr<int> p(new int(2));
    std::thread t(f4, std::move(p));
    t.join();
}

static void test12() {
    std::cout << "=-=-= test12 =-=-=\n";

    std::thread t1(f);
    std::thread t2 = std::move(t1);
    t1 = std::thread(f);
    std::thread t3 = std::move(t2);
    // t1 = std::move(t3); // t1已有关联g的线程，调用std::terminate终止程序

    t1.join();
    // t2.join();
    t3.join();
}

static std::thread test13() {
    std::cout << "=-=-= test13 =-=-=\n";
    // 线程所有权可以转移到函数外
    return std::thread(f2, 5);
}

// std::thread 也能作为参数
static void test14(std::thread t) {
    std::cout << "=-=-= test14 =-=-=\n";

    t.join();
}

/**
 * RAII
 * 与之前不同的是线程全权交给 ScopedThread 类管理
 */
class ScopedThread {
public:
    explicit ScopedThread(std::thread& t) : t_(std::move(t)) {
        if (!t_.joinable()) {
            throw std::logic_error("no thread");
        }
    }
    ~ScopedThread() {
        if (t_.joinable()) {
            t_.join();
        }
    }

private:
    ScopedThread(const ScopedThread&) = delete;
    ScopedThread& operator=(const ScopedThread&) = delete;

private:
    std::thread t_;
};

static void test15() {
    std::cout << "=-=-= test15 =-=-=\n";

    std::thread t(f);
    ScopedThread sct(t);
}

struct JoinThread {
public:
    JoinThread() noexcept = default;

    template <typename F, typename... T>
    explicit JoinThread(F&& f, T&&... args) : t_(std::forward<F>(f), std::forward<T>(args)...) {}

    explicit JoinThread(std::thread t) noexcept : t_(std::move(t)) {}

    JoinThread(JoinThread&& rhs) noexcept : t_(std::move(rhs.t_)) {}

    JoinThread& operator=(JoinThread&& rhs) noexcept {
        if (joinable()) {
            join();
        }
        t_ = std::move(rhs.t_);
        return *this;
    }

    JoinThread& operator=(std::thread rhs) noexcept {
        if (joinable()) {
            join();
        }
        t_ = std::move(rhs);
        return *this;
    }

    ~JoinThread() {
        if (joinable()) {
            t_.join();
        }
    }

    void swap(JoinThread&& rhs) noexcept { t_.swap(rhs.t_); }

    std::thread::id get_id() const noexcept { return t_.get_id(); }

    bool joinable() const noexcept{
        return t_.joinable();
    }

    void join() {
        t_.join();
    }

    void detach() {
        t_.detach();
    }

    std::thread& as_thread() noexcept { return t_; }

    const std::thread& as_thread() const noexcept { return t_; }

private:
    std::thread t_;
};

static void test16() {
    std::cout << "=-=-= test16 =-=-=\n";

    std::vector<std::thread> ths;
    for (int i = 0; i < 4; ++i) {
        ths.push_back(std::thread(f));
    }
    
    // mem_fn 将成员函数转为函数对象
    std::for_each(ths.begin(), ths.end(), std::mem_fn(&std::thread::join));
}

static void test17() {
    std::cout << "=-=-= test17 =-=-=\n";

    std::cout << "机器支持的并发数：" << std::thread::hardware_concurrency() << std::endl;
}

static void test19() {
    std::cout << "=-=-= test19 =-=-=\n";
    std::cout << std::this_thread::get_id() << std::endl;
}

// ------------- 并行版的 accumulate -------------
template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& res)
    {
        res = std::accumulate(first, last, res);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    const unsigned long len = std::distance(first, last);
    if (!len) return init;
    const unsigned long min_per_thread = 25;
    // 对需要的线程，做向上取整
    const unsigned long max_threads = (len + min_per_thread - 1) / min_per_thread;
    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    const unsigned long num_threads = // 线程数量
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    const unsigned long block_size = len / num_threads; // 每个线程中的数据量
    std::vector<T> res(num_threads);
    std::vector<std::thread> threads(num_threads - 1); // 已有一个主线程，所以少一个线程
    Iterator block_start = first;
    for (unsigned long i = 0; i < num_threads - 1; ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size); // block_end指向当前块的尾部
        threads[i] = std::thread(accumulate_block<Iterator, T>{},
            block_start, block_end, std::ref(res[i]));
        block_start = block_end;
    }
    accumulate_block<Iterator, T>()(block_start, last, res[num_threads - 1]);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(res.begin(), res.end(), init);
}

int main(int argc, char** argv) {
    std::cout << "=-=-= test begin =-=-=\n";

    std::cout << "=-=-= 线程管理基础 =-=-=\n";


    // test();

    // test2();

    // test3();
    // ::sleep(3);

    // test4();

    // test5();

    // test6();

    // test7();

    std::cout << "=-=-= 为线程传递参数 =-=-=\n";

    // test8();

    // test9();

    // test10();

    // test11();

    std::cout << "=-=-= 转移线程所有权 =-=-=\n";

    // test12();

    // std::thread t = test13();
    // t.join();

    // std::thread t(f);
    // test14(std::move(t));

    // test15();

    // test16();

    std::cout << "=-=-= 运行期选择线程数量 =-=-=\n";

    // test17();

    std::cout << "=-=-= 线程 id =-=-=\n";

    test19();

    std::cout << "=-=-= test end =-=-=\n";
    return 0;
}

// https://downdemo.gitbook.io/cpp-concurrency-in-action-2ed/1.-xian-cheng-guan-li-managing-thread/xian-cheng-guan-li-ji-chu
