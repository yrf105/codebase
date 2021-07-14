#include <iostream>

class Day {
public:
    explicit Day(int day) : day_(day) {}

private:
    int day_;
};

class Month {
public:
    static Month Jan() { return Month(1); }
    static Month Feb() { return Month(2); }
    static Month Mar() { return Month(3); }
    static Month Apr() { return Month(4); }
    static Month May() { return Month(5); }
    static Month Jun() { return Month(6); }
    static Month Jul() { return Month(7); }
    static Month Aug() { return Month(8); }
    static Month Sep() { return Month(9); }
    static Month Oct() { return Month(10); }
    static Month Nov() { return Month(11); }
    static Month Dec() { return Month(12); }

private:
    explicit Month(int month) : month_(month) {}
private:
    int month_;
};

class Year {
public:
    explicit Year(int year) : year_(year) {}

private:
    int year_;
};

class Date {
public:
    explicit Date(Day day, Month month, Year year)
        : day_(day), month_(month), year_(year) {}

private:
    Day day_;
    Month month_;
    Year year_;
};

int main() {
    // error: no instance of constructor "Date::Date" matches the argument list
    // -- argument types are: (int, int, int)
    // Date d1(5, 6, 2021);

    // error: no instance of constructor "Date::Date" matches the argument list
    // -- argument types are: (Month, Day, Year)
    // Date d2(Month(6), Day(5), Year(2021));

    // 正确
    Date d3(Day(5), Month::Jun(), Year(2021));

    return 0;
}