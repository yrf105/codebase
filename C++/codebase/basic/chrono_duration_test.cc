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

    std::this_thread::sleep_for(std::chrono::seconds(1));    

    std::cout << (now() - start).count() << std::endl; // 1000159500
    std::cout << (timediff_t(now() - start)).count() << std::endl; // 1.00028

    auto mytime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&mytime), "%Y-%m-%d %X") << std::endl;

    return 0;
}