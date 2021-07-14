#include <cstring>
#include <iostream>

// 该基类对应的是 StringValue 而不是 String
class RCObject {
public:
    RCObject();
    RCObject(const RCObject&);
    virtual ~RCObject() = 0;
    RCObject& operator=(const RCObject&);

    void addReference();       // 增加引用次数
    void removeReference();    // 移除引用次数
    void markUnShareable();    // 标记为不可共享
    bool isShareable() const;  // 查询共享状态
    bool isShared() const;     // 目前是否正在被共享

private:
    int refCount_;
    bool shareable_;
};

// 将 refCount_ 初始化为 0，让对象创建者将 refCount_ 设为 1，可以简化代码
RCObject::RCObject() : refCount_(0), shareable_(true) {}

// 将 refCount_ 设为 0 是因为调用 拷贝构造函数 会创建出新对象，既然是新对象
// 一定尚未被共享，且只被其创建者引用，创建者有责任为 refCount 设置合适的值
RCObject::RCObject(const RCObject&) : refCount_(0), shareable_(true) {}

RCObject::~RCObject() {}

// 通常一个「引用计数」对象不会被赋值给另一个对象，就像 StringValue 对象
// 不会被赋值一样，赋值的是 String，此时 StringValue 的实值不变，引用计数
// 会改变
// 正确行为就是什么都不做 P211 改变的是实值而不是引用计数值
RCObject& RCObject::operator=(const RCObject&) { return *this; }

void RCObject::addReference() { ++refCount_; }

// removeReference 不仅负责递减引用计数，还负责删除对象
// 所以 RCObject 必须是堆对象
void RCObject::removeReference() {
    if (--refCount_ == 0) {
        delete this;
    }
}

void RCObject::markUnShareable() { shareable_ = false; }

bool RCObject::isShareable() const { return shareable_; }

bool RCObject::isShared() const { return refCount_ > 1; }

// 类型 T 必须继承 RCObject
template <typename T>
class RCPtr {
public:
    RCPtr(T* realPtr = 0);
    RCPtr(RCPtr& rhs);
    ~RCPtr();
    RCPtr& operator=(const RCPtr& rhs);

    T* operator->() const;
    T& operator*() const;

private:
    void init();

private:
    T* pointee_;
};

template <typename T>
RCPtr<T>::RCPtr(T* realPtr) : pointee_(realPtr) {
    init();
}

template <typename T>
RCPtr<T>::RCPtr(RCPtr& rhs) : pointee_(rhs.pointee_) {
    init();
}

template <typename T>
RCPtr<T>::~RCPtr() {
    if (pointee_) {
        pointee_->removeReference();
    }
}

template <typename T>
RCPtr<T>& RCPtr<T>::operator=(const RCPtr<T>& rhs) {
    if (pointee_ != rhs.pointee_) {
        if (pointee_) {
            pointee_->removeReference();
        }
        pointee_ = rhs.pointee_;
        init();
    }
    return *this;
}

template <typename T>
T* RCPtr<T>::operator->() const {
    return pointee_;
}

template <typename T>
T& RCPtr<T>::operator*() const {
    return *pointee_;
}

template <typename T>
void RCPtr<T>::init() {
    if (pointee_ == nullptr) {
        return;
    }

    if (pointee_->isShareable() == false) {
        // 这个操作会调用 T 类型的「拷贝构造函数」
        // T 对应于上述的 StringValue，StringValue 包含指针成员
        // 所以需要为 StringValue 提供一个深拷贝的「拷贝构造函数」
        pointee_ = new T(*pointee_);
    }

    pointee_->addReference();
}

class String {
public:
    class CharProxy {
    public:
        CharProxy(String& str, int index);
        // 左值运用
        CharProxy& operator=(const CharProxy& rhs);
        CharProxy& operator=(char c);

        // 右值运用
        operator char() const;

    private:
        String& theString_;
        int charIndex_;
    };

    friend class CharProxy;

public:
    String(const char* data = "");

    // 使用编译器生成的即可
    // 编译器生成的会自动调用 value_ 的
    // value_ 是 smart pointer，会处理好这一切
    // String(const String& rhs);
    // ~String();
    // String& operator=(const String& rhs);

    const CharProxy operator[](size_t idx) const;
    CharProxy operator[](size_t idx);

private:
    // 结构体，便于 String 类内的成员函数访问
    struct StringValue : public RCObject {
        char* data_;  // 字符串值

        StringValue(const char* data);
        StringValue(const StringValue& rhs);
        void init(const char* data);
        ~StringValue();
    };

private:
    RCPtr<StringValue> value_;
};

String::CharProxy::CharProxy(String& str, int index)
    : theString_(str), charIndex_(index) {}

String::CharProxy& String::CharProxy::operator=(const CharProxy& rhs) {
    if (theString_.value_->isShared()) {
        theString_.value_ = new StringValue(theString_.value_->data_);
    }

    theString_.value_->data_[charIndex_] = rhs.theString_.value_->data_[rhs.charIndex_];
    return *this;
}

String::CharProxy& String::CharProxy::operator=(char c) {
    if (theString_.value_->isShared()) {
        theString_.value_ = new StringValue(theString_.value_->data_);
    }

    theString_.value_->data_[charIndex_] = c;
    return *this;
}


String::CharProxy::operator char() const {
    return theString_.value_->data_[charIndex_];
}

void String::StringValue::init(const char* data) {
    data_ = new char[strlen(data) + 1];
    strcpy(data_, data);
}

String::StringValue::StringValue(const char* data) { init(data); }

String::StringValue::~StringValue() { delete[] data_; }

String::StringValue::StringValue(const StringValue& rhs) { init(rhs.data_); }

String::String(const char* data) : value_(new StringValue(data)) {}

// 直接返回 CharProxy 将判断读写的任务延迟到 Proxy 的「拷贝赋值运算符」
// 和 char 的隐式转换函数中
const String::CharProxy String::operator[](size_t idx) const {
    return CharProxy(const_cast<String&>(*this), idx);
}

String::CharProxy String::operator[](size_t idx) {
    return CharProxy(*this, idx);
}

template <typename T>
class RCIPtr {
public:
    RCIPtr(T* realPtr = nullptr);
    RCIPtr(const RCIPtr& rhs);
    ~RCIPtr();

    RCIPtr& operator=(const RCIPtr& rhs);

    // 重载 * ->
    // 只要有 non-const 访问就会执行 COW
    const T* operator->() const;
    T* operator->();
    const T& operator*() const;
    T& operator*();

private:
    struct CountHolder : public RCObject {
        ~CountHolder() { delete pointee_; }
        T* pointee_;
    };

private:
    void init();
    void makeCopy();

private:
    // CounterHolder* 而不是 T*
    // T* 在 CounterHolder 里
    CountHolder* counter_;
};

template <typename T>
RCIPtr<T>::RCIPtr(T* realPtr) : counter_(new CountHolder) {
    counter_->pointee_ = realPtr;
    init();
}

template <typename T>
RCIPtr<T>::RCIPtr(const RCIPtr& rhs) : counter_(rhs.counter_) {
    init();
}

template <typename T>
RCIPtr<T>::~RCIPtr() {
    counter_->removeReference();
}

template <typename T>
RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr& rhs) {
    if (counter_ != rhs.counter_) {
        counter_->removeReference();
        counter_ = rhs.counter_;
        init();
    }
    return *this;
}

template <typename T>
const T* RCIPtr<T>::operator->() const {
    return counter_->pointee_;
}

template <typename T>
const T& RCIPtr<T>::operator*() const {
    return *(counter_->pointee_);
}

template <typename T>
T* RCIPtr<T>::operator->() {
    makeCopy();  // cow
    return counter_->pointee_;
}

template <typename T>
T& RCIPtr<T>::operator*() {
    makeCopy();  // cow
    return *(counter_->pointee_);
}

template <typename T>
void RCIPtr<T>::init() {
    if (counter_->isShareable() == false) {
        T* oldValue = counter_->pointee_;
        counter_ = new CountHolder;
        counter_->pointee_ = new T(*oldValue);
    }
    counter_->addReference();
}

// 写时复制
template <typename T>
void RCIPtr<T>::makeCopy() {
    if (counter_->isShared()) {
        T* oldValue = counter_->pointee_;
        counter_->removeReference();
        counter_ = new CountHolder;
        counter_->pointee_ = new T(*oldValue);
        counter_->addReference();
    }
}

int main() { return 0; }