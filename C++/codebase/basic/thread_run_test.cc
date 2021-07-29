#include <iostream>
#include <thread>
#include <memory>

class Foo {
public:
    Foo() {

    }
    ~Foo() {
        thread_->join();
    }

    void run() {
        thread_ = std::make_unique<std::thread>([]{
            std::cout << "run" << std::endl;
        });
    }

private:
    std::unique_ptr<std::thread> thread_;
};

int main() {

    Foo f;
    f.run();

    return 0;
}