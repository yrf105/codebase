#include <iostream>
#include <string>
#include <new>

class A {
public:
    A() {
        std::cout << "A()\n";
    }
    ~A() {
        std::cout << "~A\n";
    }

private:
    std::string name_;
};

class Widget {
public:
    Widget(const std::string& str) : str_(str) {}
    static Widget* ConstructWidgetInBuffer(void* buffer, const std::string& str) {
        return new(buffer)Widget(str);
    }
private:
    std::string str_;
};

int main() {
    // A a;
    // a.~A();

    void* buffer = ::malloc(sizeof(Widget));

    Widget* pw = Widget::ConstructWidgetInBuffer(buffer, "yyy");

    pw->~Widget();
    free(buffer);

    return 0;
}