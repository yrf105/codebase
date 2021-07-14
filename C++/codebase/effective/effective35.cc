#include <functional>
#include <iostream>

class GameCharacter;

class HealthCalcFunc {
public:
    virtual int calc(const GameCharacter& gc) const;
};

class GameCharacter {
public:
    GameCharacter(HealthCalcFunc* hcf) : healthCalcFunc_(hcf) {}
    int healthValue() { return healthCalcFunc_->calc(*this); }

private:
    HealthCalcFunc* healthCalcFunc_;
};

int main() { return 0; }