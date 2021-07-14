#include <unistd.h>

#include <iostream>

class Timer {
public:
    explicit Timer(int frequency = 1) : frequency_(frequency) {}
    // 定时器每滴答一次
    // 该函数就被调用一次
    virtual void onTick() const { std::cout << "Timer::onTick\n"; }

private:
    int frequency_;
};

// 使用 复合 代替 private 继承
class Widget {
public:
    Widget() : wt_() {}

    void foo() {
        wt_.onTick();
    }

private:
    class WidgetTimer : public Timer {
    public:
        void onTick() const override { std::cout << "WidgetTimer::onTick\n"; }
    private:
    };

private:
    WidgetTimer wt_;
};


class EmptyBase {
public:
    using INT = int;
    void foo() {}
};

class Derived {
private:
    int i_;
    EmptyBase eb_;
};

int main() {
    
    std::cout << sizeof(EmptyBase) << std::endl; // 1
    std::cout << sizeof(Derived) << std::endl; // 8

    return 0;
}