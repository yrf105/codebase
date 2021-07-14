#include <iostream>
#include <cstring>
#include <string>

class Text {
public:
    Text(const char* text) {
        size_t n = sizeof(char)*(strlen(text) + 1);
        text_ = new char[n];
        memcpy(text_, text, n);
    }
    ~Text() {
        if (text_) {
            delete[] text_;
        }
    }

    Text& operator=(const Text& rhs) {
        if (this == &rhs) {
            return *this;
        }
        delete[] text_;
        size_t n = sizeof(char)*(strlen(rhs.text_) + 1);
        text_ = new char[n];
        memcpy(text_, rhs.text_, n);
        return *this;
    }

    std::string text() const {
        return text_;
    }

private:
    char* text_;
};

int main() {
    Text t1("hello");
    std::cout << t1.text() << std::endl; // hello
    Text t2("world");
    std::cout << t2.text() << std::endl; // world

    t2 = t1;
    std::cout << t1.text() << std::endl; // hello
    std::cout << t2.text() << std::endl; // hello

    t1 = t1;
    // 什么也没有，错误的自赋值导致『在停止使用资源之前意外释放了他』
    std::cout << t1.text() << std::endl; //  

    return 0;
}