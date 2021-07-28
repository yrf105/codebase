#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>

#include <thread>
int main(int argc, char** argv) {

    auto now = []() -> auto {
        return std::chrono::high_resolution_clock::now();
    };

    using timediff_t = std::chrono::duration<long double>;

    const auto start = now();

    // std::this_thread::sleep_for(std::chrono::seconds(1));    

    std::cout << (now() - start).count() << std::endl; // 1000159500
    std::cout << (timediff_t(now() - start)).count() << std::endl; // 1.00028

    auto mytime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&mytime), "%Y-%m-%d %X") << std::endl;


    std::chrono::seconds s = std::chrono::seconds(1);
    std::cout << s.count() << std::endl;
    std::chrono::nanoseconds ns = std::chrono::seconds(1);
    std::cout << ns.count() << std::endl;
    std::cout << (ns + s).count() << std::endl;

    std::chrono::system_clock::time_point when = std::chrono::system_clock::now() + ns + s;
    std::cout << when.time_since_epoch().count() << std::endl;
    std::chrono::nanoseconds nns = (when - std::chrono::system_clock::now());
    std::cout << nns.count() << std::endl;
    std::cout << (when.time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
    nns = std::chrono::nanoseconds(100000);
    std::cout << nns.count() << std::endl;

    std::this_thread::sleep_until(when);
    std::cout << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();
    std::cout << (tp1 < tp2) << std::endl;

    std::cout << std::chrono::system_clock::time_point{}.time_since_epoch().count() << std::endl;

    auto interval1 = std::chrono::seconds{};
    auto interval2 = std::chrono::nanoseconds(0);
    std::cout << (interval1 == interval2) << std::endl;

    interval2 = interval1;

    return 0;
}