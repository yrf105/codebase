#include <chrono>
#include <iostream>

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

    return 0;
}