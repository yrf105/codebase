#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <functional>
#include <iomanip>      // std::put_time
#include <list>

std::mutex mu;
std::condition_variable cv;
int n = 0;
bool ready = false;
bool processed = false;

static void f() {
    std::unique_lock<std::mutex> lock(mu);
    cv.wait(lock, []() { return ready; });
    ++n;
    processed = true;
    lock.unlock();
    cv.notify_one();
}

static void test() {
    std::cout << "=-=-= test =-=-=\n";

    std::thread t1(f);

    {
        std::lock_guard<std::mutex> lock(mu);
        n = 5;
        ready = true;
        cv.notify_one();
    }

    {
        std::unique_lock<std::mutex> lock(mu);
        cv.wait(lock, []() { return processed; });
    }

    std::cout << n << std::endl;

    t1.join();
}

// -------------------- 条件变量实现线程安全的 queue --------------------
template <typename T>
class Queue {
private:
    mutable std::mutex mu_;
    std::condition_variable cv_;
    std::queue<T> queue_;

public:
    Queue() : queue_(std::queue<T>()) {}

    Queue(const Queue& rhs) {
        std::lock_guard lock(rhs.mu_);
        queue_ = rhs.queue_;
    }

    void push(T t) {
        std::lock_guard lock(mu_);
        queue_.push(std::move(t));
        cv.notify_one();
    }

    bool try_pop(T& v) {
        std::lock_guard lock(mu_);
        if (queue_.empty()) {
            return false;
        }

        v = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard lock(mu_);
        if (queue_.empty()) {
            return std::shared_ptr<T>();
        }

        std::shared_ptr<T> ret(std::make_shared<T>(std::move(queue_.front())));
        queue_.pop();
        return ret;
    }

    void wait_and_pop(T& v) {
        std::unique_lock<std::mutex> lock(mu_);
        cv.wait(mu_, [this] { return !queue_.empty(); });
        v = std::move(queue_.front());
        queue_.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lock(mu_);
        cv.wait(mu_, [this] { return !queue_.empty(); });
        std::shared_ptr<T> ret(std::make_shared<T>(std::move(queue_.front())));
        queue_.pop();
    }

    bool empty() {
        std::lock_guard lock(mu_);
        return queue_.empty();
    }
};

Queue<std::string> q;

static void foo1() {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        q.push(std::to_string(i));
    }
}

static void test1() {
    std::cout << "=-=-= test1 =-=-=\n";

    std::thread t1(foo1);
    std::thread t2(foo1);

    t1.join();
    t2.join();

    std::string str;
    while (q.try_pop(str)) {
        std::cout << str << std::endl;
    }
}

static int foo2() {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 5;
}

static void test2() {
    std::cout << "=-=-= test2 =-=-=\n";

    std::future<int> ft = std::async(foo2);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << ft.get() << std::endl;
}

static int foo3() { return 5; }

static void test3() {
    std::cout << "=-=-= test3 =-=-=\n";

    // 使用 std::packaged_task 关联 future 和 任务
    std::packaged_task<int()> pt(foo3);
    auto ft = pt.get_future();
    pt();
    std::cout << ft.get() << std::endl;
}

// ---------------------- 使用指定线程更新 GUI ----------------------
// std::mutex m;
// std::deque<std::packaged_task<void()>> d;

// void gui_thread() // 更新GUI的指定线程
// {
//     while (!gui_shutdown_message_received()) // 未收到终止消息则一直轮询
//     {
//         process_gui_message(); // 处理收到的消息
//         std::packaged_task<void()> pt;
//         {
//             std::lock_guard<std::mutex> l(m);
//             if (d.empty()) continue; // 进入下一次循环
//             pt = std::move(d.front());
//             d.pop_front();
//         }
//         pt();
//     }
// }

// std::thread t(gui_thread);

// template<typename F>
// std::future<void> postTask(F f)
// {
//     std::packaged_task<void()> pt(f);
//     std::future<void> res = pt.get_future();
//     std::lock_guard<std::mutex> l(m);
//     d.push_back(std::move(pt));
//     return res;
// }

static void test4() {
    std::cout << "=-=-= test4 =-=-=\n";

    std::promise<int> ps;
    auto ft = ps.get_future();
    ps.set_value(42);  // 使用 promise 可以显示设置值
    std::cout << ft.get() << std::endl;
}

static void foo5(std::promise<void>& ps) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    ps.set_value();
}

// 在线程间对状态发送信号
static void test5() {
    std::cout << "=-=-= test5 =-=-=\n";

    std::promise<void> ps;
    std::thread t(foo5, std::ref(ps));
    auto ft = ps.get_future();
    ft.wait();  //  // 阻塞直到set_value，相当于没有返回值的get
    std::cout << "test5() end\n";
    t.join();
}

static int foo6(int x) {
    if (x < 0) {
        throw std::out_of_range("x < 0");
    }
    return x;
}

static void test6() {
    std::cout << "=-=-= test6 =-=-=\n";
    std::future<int> ft = std::async(foo6, -1); // ft将存储异常
    std::cout << ft.get() << std::endl; // 抛出已存储的异常
}

static void test7() {
    std::cout << "=-=-= test7 =-=-=\n";

    std::promise<int> ps;
    auto ft = ps.get_future();
    std::thread t([&ps]{
        try {
            ps.set_value(foo6(-1)); // 此时还没有存储异常
        } catch(...) {
            ps.set_exception(std::current_exception()); // 存储异常
        }
    });
    t.join();
    ft.get();
}

// 如果std::packaged_task和std::promise直到析构都未设置值，
// std::future::get也会抛出std::future_error异常
static void test8() {
    std::cout << "=-=-= test8 =-=-=\n";

    std::future<int> ft;
    {
        std::packaged_task<int()> pt(std::bind(foo6, -1));
        ft = pt.get_future();
    }
    ft.get(); // // 抛出异常
}

static void test9() {
    std::cout << "=-=-= test9 =-=-=\n";
    
    // 打印当前系统时间
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&now_c), "%F %T\n");
}

static void foo10() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// 使用 duration 设置等待时间
static void test10() {
    std::cout << "=-=-= test10 =-=-=\n";

    auto ft = std::async(foo10);
    if (ft.wait_for(std::chrono::milliseconds(1200)) == std::future_status::ready) {
        std::cout << "666\n";
    }
}

// 使用绝对时间来设置等待时间
static void test11() {
    const auto time_out = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    std::condition_variable cv;
    std::mutex mu;

    std::unique_lock<std::mutex> lock(mu);
    while (true) {
        if (cv.wait_until(lock, time_out) == std::cv_status::timeout) {
            break;
        }
    }
    std::cout << "timeout...\n";
}

template <typename T>
std::list<T> foo12(std::list<T> li) {
    if (li.empty()) {
        return li;
    }

    std::list<T> res;
    res.splice(res.begin(), li, li.begin());
    /**
     *  [] 不截取任何变量
        [&] 截取外部作用域中所有变量，并作为引用在函数体中使用
        [=] 截取外部作用域中所有变量，并拷贝一份在函数体中使用
    */
    const T& firstvalue = *res.begin();
    auto it = std::partition(li.begin(), li.end(), [&](const T& x){ return x < firstvalue; });

    std::list<T> low;
    low.splice(low.end(), li, li.begin(), it);
    auto l(foo12(std::move(low)));
    auto r(foo12(std::move(li)));
    res.splice(res.end(), r);
    res.splice(res.begin(), l);
    return res;
}

template <typename T>
std::list<T> foo12s(std::list<T> li) {
    if (li.empty()) {
        return li;
    }

    std::list<T> res;
    res.splice(res.begin(), li, li.begin());
    /**
     *  [] 不截取任何变量
        [&] 截取外部作用域中所有变量，并作为引用在函数体中使用
        [=] 截取外部作用域中所有变量，并拷贝一份在函数体中使用
    */
    const T& firstvalue = *res.begin();
    auto it = std::partition(li.begin(), li.end(), [&](const T& x){ return x < firstvalue; });

    std::list<T> low;
    low.splice(low.end(), li, li.begin(), it);
    std::future<std::list<T>> l(std::async(&foo12s<T>, std::move(low)));
    auto r(foo12s(std::move(li)));
    res.splice(res.end(), r);
    res.splice(res.begin(), l.get());
    return res;
}

static void test12() {
    std::list<int> l{2,3,1,5,7,4,6};
    std::list l2(foo12s(l));
    for (auto i : l2) {
        std::cout << i << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "=-=-= test begin =-=-=\n";

    std::cout << "=-=-= 等待一个事件或其他条件 =-=-=\n";
    // 条件变量
    // test();

    std::cout << "=-=-= 用条件变量实现线程安全的queue =-=-=\n";
    // test1();

    std::cout << "=-=-= 使用期值等待一次性事件 =-=-=\n";
    std::cout << "=-=-= std::async =-=-=\n";
    // std::async 同样支持函数，函数对象，lambda，成员函数
    // std::async还可以设置第一个参数为线程的创建策略
    // int f();
    // // 函数必须异步执行，即运行在不同的线程上
    // auto ft1 = std::async(std::launch::async, f);
    // // 函数只在返回的期值调用get或wait时运行
    // auto ft2 = std::async(std::launch::deferred, f);
    // // 不指定时的默认启动策略是对两者进行或运算的结果
    // // auto ft3 = std::async(f)等价于
    // auto ft3 = std::async(std::launch::async | std::launch::deferred, f);
    // test2();

    std::cout << "=-=-= std::packaged_task =-=-=\n";
    // test3();

    std::cout << "=-=-= std::promise =-=-=\n";
    // 一个std::promise只能关联一个std::future，
    // 关联多次时将抛出std::future_error异常
    // test4();

    // test5();

    std::cout << "=-=-= 将异常储存于期值中 =-=-=\n";
    // test6();

    // test7();

    // test8();

    std::cout << "=-=-= std::shared_future =-=-=\n";
    // std::future调用get后就无法再次get，
    // 也就是说只能获取一次数据，
    // 此外还会导致所在线程与其他线程数据不同步。
    // std::shared_future就可以解决此问题

    // std::promise<int> ps;
    // std::future<int> ft(ps.get_future());
    // assert(ft.valid());
    // std::shared_future<int> sf(std::move(ft));
    // assert(!ft.valid());
    // assert(sf.valid());

    // std::promise<int> ps;
    // // std::future隐式转换为std::shared_future
    // std::shared_future<int> sf(ps.get_future());

    // std::promise<int> ps;
    // auto sf = ps.get_future().share();

    // 每一个std::shared_future对象上返回的结果不同步，
    // 为了避免多线程访问同一std::shared_future对象时的数据竞争就必须加锁保护。
    // 更好的方法是给每个线程拷贝一个std::shared_future对象，
    // 这样就可以安全访问而无需加锁

    std::cout << "=-=-= 限定等待时间 =-=-=\n";
    std::cout << "=-=-= 时钟 =-=-=\n";
    // test9();

    std::cout << "=-=-= std::chrono::duration =-=-=\n";
    // // 比如将表示秒的类型定义为
    // std::duration<int> // 即std::chrono::seconds
    // // 则表示分的类型可定义为
    // std::duration<int, std::ratio<60>> // 即std::chrono::minutes
    // // 表示毫秒的类型可定义为
    // std::duration<int, std::ratio<1, 1000>> // 即std::chrono::milliseconds

    // C++14的std::chrono_literals提供了表示时间的后缀
    // using namespace std::chrono_literals;
    // auto x = 45min; // 等价于std::chrono::minutes(45)
    // std::cout << x.count(); // 45
    // auto y = std::chrono::duration_cast<std::chrono::seconds>(x);
    // std::cout << y.count(); // 2700
    // auto z = std::chrono::duration_cast<std::chrono::hours>(x);
    // std::cout << z.count(); // 0（转换会截断）

    // 标准库通过字面值运算符模板实现此后缀功能
    // constexpr std::chrono::minutes operator ""min(unsigned long long m)
    // {
    //     return std::chrono::minutes(m);
    // }

    // // duration支持四则运算
    // using namespace std::chrono_literals;
    // auto x = 1h;
    // auto y = 15min;
    // auto z = x - 2 * y;
    // std::cout << z.count(); // 30

    // test10();

    std::cout << "=-=-= std::chrono::time_point =-=-=\n";
    // 第一个模板参数为开始时间点的时钟类型，第二个为时间单位
    // std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>

    // time_point可以加减dutation
    // using namespace std::chrono_literals;
    // auto x = std::chrono::high_resolution_clock::now();
    // auto y = x + 1s;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(y - x).count();

    // 两个time_point也能相减
    // auto start = std::chrono::high_resolution_clock::now();
    // doSomething();
    // auto stop = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    // test11();
    // 很多函数都支持 timeout

    std::cout << "=-=-= 使用同步操作简化代码 =-=-=\n";
    std::cout << "=-=-= 使用期值进行函数式编程 =-=-=\n";

    // test12();

    std::cout << "=-=-= 使用消息传递进行同步操作 =-=-=\n";
    // 见 /home/wddxrw/workspace/codebase/C++/codebase/concurrent/examples/ATM.cc

    std::cout << "=-=-= std::experimental::future =-=-=\n";
    // 使得 future 具有持续性，不必等待，数据就绪后进行处理

    std::cout << "=-=-= std::experimental::when_all =-=-=\n";
    // 使用std::experimental::when_all可以避免反复唤醒导致的开销，
    // 为其传入一组需要等待的期值，将返回一个新的期值。
    // 当传入的所有期值都就绪时，则返回的期值就绪

    std::cout << "=-=-= std::experimental::when_any =-=-=\n";
    // 在传入的期值中有一个就绪时，则std::experimental::when_any返回的期值就绪
    
    std::cout << "=-=-= std::experimental::latch =-=-=\n";
    // 等待计数器减为零，再开始执行某个线程

    std::cout << "=-=-= std::experimental::barrier =-=-=\n";
    // 一组处理数据的线程，处理过程中独立，无需同步，但在处理下一项数据前，必须要求所有线程完成任务。

    std::cout << "=-=-= std::experimental::flex_barrier =-=-=\n";
    // 基本同 barrier，还可以传入一个参数在线程集执行完毕后由其中一个线程执行此函数

    std::cout << "=-=-= test end =-=-=\n";
    return 0;
}