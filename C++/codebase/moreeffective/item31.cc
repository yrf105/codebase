#include <iostream>
#include <typeinfo>
#include <map>
#include <memory>

class B {
public:
    virtual ~B() {};
};

class D1 : public B {
public:
};

class D2 : public B {
public:
};

namespace {
    void barD1D2(B& d1, B& d2) {
        std::cout << "D1 D2\n";
    }
    void barD2D1(B& d1, B& d2) {
        std::cout << "D2 D1\n";
    }

    using BarFuncPtr = void(*)(B&, B&);
    using BarMap = std::map<std::pair<std::string, std::string>, BarFuncPtr>;
    std::pair<std::string, std::string> makeStrPair(const char* s1, const char* s2);
    BarFuncPtr lookup(const std::string& o1, const std::string& o2);
    BarMap* initBarMap();
}

void bar(B& o1, B& o2) {
    std::cout << "{" << typeid(o1).name() << ", " << typeid(o2).name() << "}\n";

    BarFuncPtr barFunc = lookup(typeid(o1).name(), typeid(o2).name());
    if (barFunc) {
        barFunc(o1, o2);
    } else {
        std::cout << "没有找到" << "{" << typeid(o1).name() << ", " << typeid(o2).name() << "}"  << " 对应函数\n";
        throw;
    }
}

namespace {
    std::pair<std::string, std::string> makeStrPair(const char* s1, const char* s2) {
        return std::pair<std::string, std::string>(s1, s2);
    }

    BarMap* initBarMap() {
        BarMap *ret = new BarMap;
        (*ret)[makeStrPair("2D1", "2D2")] = &barD1D2;
        (*ret)[makeStrPair("2D2", "2D1")] = &barD2D1;
        return ret;
    }

    BarFuncPtr lookup(const std::string& o1, const std::string& o2) {
        static std::unique_ptr<BarMap> barMap(initBarMap());
        auto ret = barMap->find(std::make_pair(o1, o2));
        if (ret == barMap->end()) {
            return nullptr;
        }
        return ret->second;
    }
}

void foo(B& o1, B& o2) {
    std::cout << "{" << typeid(o1).name() << ", " << typeid(o2).name() << "}\n";
    bar(o1, o2);
}

int main() {

    D1 o1;
    D2 o2;
    std::cout << "{" << typeid(o1).name() << ", " << typeid(o2).name() << "}\n";
    foo(o1, o2);
    foo(o2, o1);

    return 0;
}