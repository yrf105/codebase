#include <iostream>
#include <string>

class Transaction {
public:
    explicit Transaction(const std::string& logInfo) {
        // ...
        logTransaction(logInfo);
    }
    void logTransaction(const std::string& logInfo) const {
        std::cout << logInfo << std::endl;
    }
};

class BuyTransaction : public Transaction {
public:
    BuyTransaction() : Transaction("BuyTransaction") {}
};

class SellTransaction : public Transaction {
public:
    SellTransaction() : Transaction("SellTransaction") {}
};

int main() {
    // BuyTransaction // 调用的是适当版本
    BuyTransaction bt;

    return 0;
}