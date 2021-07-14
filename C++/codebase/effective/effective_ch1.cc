#include <iostream>

#include <string>
#include <iostream>
#include <cstring>
#include <mutex>

class TextBlock {
public:
    TextBlock(const char* text) {
        size_t n = strlen(text) + 1;
        text_ = static_cast<char*>(::malloc(n));
        ::memcpy(text_, text, n);
    }

    ~TextBlock() {
        if (text_) {
            ::free(text_);
        }
    }

    const char& operator[](size_t i) const {
        std::cout << "const char& operator[](size_t i) const\n";
        return text_[i];
    }

    char& operator[](size_t i) {
        std::cout << "char& operator[](size_t i)\n";
        return const_cast<char&>((*static_cast<const TextBlock*>(this))[i]);
    }

    // std::string text() const {
    //     std::lock_guard lock(mutex_);
    //     return text_;
    // }

private:
    char* text_;
    // mutable std::mutex mutex_;
};

void printChar(const TextBlock& tb) {
    std::cout << tb[0] << std::endl;
}

class Student {

public:
    Student(int id, const std::string& name);

private:
    int id_;
    std::string name_;
};

Student::Student(int id, const std::string& name)
    : id_(id),
      name_(name) {} // 使用成员初值列完成数据成员的初始化

int main() {

    TextBlock tb("hello");

    // char& operator[](size_t i) const
    // char& operator[](size_t i) const
    // w
    // tb[0] = 'w'; // const 成员函数修改了对象内容
    // printChar(tb);

    std::cout << tb[0] << std::endl;

    // std::cout << tb.text() << std::endl;

    // char& operator[](size_t i)
    // h
    // std::cout << tb[0] << std::endl;

    // const char& operator[](size_t i) const
    // h
    // printChar(tb);
    return 0;
}