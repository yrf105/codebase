#include <iostream>
#include <string>

std::string str1FromDB = "yyy";
std::string str2FromDB = "rrr";


class LargeObj {
public:
    LargeObj() : str1_(nullptr), str2_(nullptr) {}

    const std::string& str1() const {
        if (str1_) {
            // 假设是从数据库中读取相应 LargerObj 对象的 str1_
            // 字段来初始化
            str1_ = &str1FromDB;
        }
        return *str1_;
    }

    const std::string& str2() const {
        if (str2_) {
            // 假设是从数据库中读取相应 LargerObj 对象的 str1_
            // 字段来初始化
            str2_ = &str2FromDB;
        }
        return *str2_;
    }

private:
    mutable std::string* str1_;
    mutable std::string* str2_;
    // 假设 str1_ str2_ 都很大，且需要从数据库中读取
};

int main() { return 0; }