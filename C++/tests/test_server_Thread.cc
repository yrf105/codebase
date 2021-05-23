#include "server/Thread.h"

#include <unistd.h>

#include <iostream>
#include <string>

class TestThread : public codebase::server::object_oriented::Thread {

public:
    TestThread(int count, std::string str = "hello world") : count_(count), str_(str) {}
    ~TestThread(){}

private:
    void run() override {
        while (count_ > 0) {
            std::cout << str_ << std::endl;
            --count_;
            sleep(1);
        }
    }

private:
    int count_;
    std::string str_;
};

void foo() {
    std::cout << "hello, world\n";
}

int main(int argc, char** argv) {

    // TestThread t1(5);

    // t1.start();
    // t1.join();

    // TestThread* t2 = new TestThread(5);
    // t2->start();

    codebase::server::object_based::Thread t3(foo);
    t3.start();
    t3.join();

    return 0;
}