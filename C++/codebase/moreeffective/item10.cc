#include <iostream>
#include <memory>
#include <string>

class CPU {
public:
    using ptr = std::shared_ptr<CPU>;

    CPU(const std::string& name) : name_(name), type_() {
        std::cout << "CPU()\n";
    }
    ~CPU() { std::cout << "~CPU\n"; }

private:
    std::string name_;
    std::string type_;
};

class Screen {
public:
    using ptr = std::shared_ptr<Screen>;

    Screen(const std::string& name) : name_(name), type_() { throw "Screen"; }
    ~Screen() { std::cout << "~Screen\n"; }

private:
    std::string name_;
    std::string type_;
};

class Computer {
public:
    Computer(int no, const std::string& name, const std::string& cpuName,
             const std::string& screenName)
        : no_(no),
          name_(name),
          cpu_(cpuName == "" ? nullptr : new CPU(cpuName)),
          screen_(screenName == "" ? nullptr : new Screen(screenName)) {}

    ~Computer() { /*析构函数什么都不用做*/ }

private:
    int no_;
    std::string name_;
    CPU::ptr cpu_;
    Screen::ptr screen_;
};


// int main(int argc, char** argv) {
//     Computer* computer = nullptr;
//     try {
//         computer = new Computer(1, "pc", "intel", "aoc");
//     } catch (...) {
//         std::cout << "捕获到异常\n";
//         delete computer;
//         throw;
//     }

//     delete computer;

//     return 0;
// }

int main(int argc, char** argv) {
    // cpu_ 创建完成
    // ~CPU
    // 释放 cpu_
    // 释放 screen_
    // terminate called after throwing an instance of 'char const*'
    // [1]    62888 abort (core dumped)  ../bin/item10
    
    Computer* computer = nullptr;
    try {
        computer = new Computer(1, "pc", "intel", "aoc");
    } catch(...) {
        delete computer;
    }
    
    delete computer;
    return 0;
}