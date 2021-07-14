#include <iostream>
#include <string>

template <typename T>
class ObjectName {

public:

    ObjectName(const char* name, const T& objectValue)
        : name_(name),
          objectValue_(objectValue) {}
    ObjectName(const std::string& name, const T& objectValue)
        : name_(name),
          objectValue_(objectValue) {}

private:
    const std::string& name_;
    const T objectValue_;
};

int main() {
    // error: no default constructor exists for class "ObjectName<int>"
    // ObjectName<int> obj1; // ObjectName 已经定义了构造函数，编译器不会再为其生成默认构造函数

    // ObjectName<int> obj1("int", 0);

    // // 调用编译器生成的拷贝构造函数
    // // 对于 obj2.name_ 会调用 std::string 的以 obj1.name_ 为实参的拷贝构造函数
    // // 对于 obj2.objectValue_ 因为是 int 所以会以拷贝 obj1.objectValue_ 的每一位来初始化
    // ObjectName<int> obj2(obj1);

    // ObjectName<int> obj1("int", 0);
    // ObjectName<int> obj2 = obj1; // 这里调用的是拷贝构造函数
    // error: function "ObjectName<T>::operator=(const ObjectName<int> &) [with T=int]" 
    // (declared implicitly) cannot be referenced -- it is a deleted functionC/C++(1776)
    // obj2 = obj1;

    return 0;
}