#include <iostream>
#include <vector>

template <typename T>
class WidgetImpl {
public:
private:
    int a_;  // 有很多数据成员，复制需要很多时间
    std::vector<int> vec_;
    T t_;
    // ...
};

template <typename T>
class Widget {
public:
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs) {
        // ...
        *pImpl_ = *(rhs.pImpl_);
        // ...
    }

    // Widget 提供只交换指针的 public swap 成员函数
    void swap(Widget& rhs) {
        using std::swap;  // 这个 swap 是必须的
        swap(pImpl_, rhs.pImpl_);
    }

private:
    WidgetImpl<T>* pImpl_;
};

// error: non-class, non-variable partial specialization ‘swap<Widget<T> >’ is
// not allowed
template <typename T>
void swap(Widget<T>& lhs, Widget<T>& rhs) {
    lhs.swap(rhs);
}

int main() { return 0; }