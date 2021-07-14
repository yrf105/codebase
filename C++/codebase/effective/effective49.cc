#include <new>
#include <boost/noncopyable.hpp>

class NewHandlerHolder : boost::noncopyable {
public:
    NewHandlerHolder(std::new_handler oh) : oldHandler_(oh) {}
    ~NewHandlerHolder() {
        std::set_new_handler(oldHandler_);
    }
private:
    std::new_handler oldHandler_;
};

template <typename T>
class NewHandlerSupport {
public:
    static std::new_handler set_new_handler(std::new_handler p) throw();
    static void* operator new(std::size_t size) _GLIBCXX_THROW (std::bad_alloc);
    // 其他版本的 operator new
private:
    static std::new_handler currentNewHandler_;
};

template <typename T>
std::new_handler NewHandlerSupport<T>::currentNewHandler_ = nullptr;

template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw() {
    std::new_handler oldNewHandler = currentNewHandler_;
    currentNewHandler_ = p;
    return oldNewHandler;
}

template <typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) _GLIBCXX_THROW (std::bad_alloc) {
    NewHandlerHolder holder(std::set_new_handler(currentNewHandler_));
    return ::operator new(size);
}

class Widget : public NewHandlerSupport<Widget> {

};

int main() {
    return 0;
}