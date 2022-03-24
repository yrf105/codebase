#include <iostream>
#include <algorithm>
#include <string.h>

struct P {
    int id = 0;
};

struct A {
    // int i;
    int a[0];
};

int main() {

    P ps[5];
    ps[1].id = 1;
    ps[2].id = 7;

    std::cout << ps[1].id << std::endl;
    std::cout << ps[2].id << std::endl;

    P target{0};

    auto it = std::lower_bound(ps, ps + 2, target, [](const P& lhs, const P& rhs){
        return lhs.id < rhs.id;
    });

    std::cout << it - ps << std::endl;

    A* array;
    char data[12]{'0'};
    std::cout << "sizeof(data): " << sizeof(data) << std::endl;
    // data[0] = '1';
    // data[1] = '0';
    // data[2] = '0';
    // data[3] = '0';
    // data[4] = '1';
    // data[5] = '0';
    // data[6] = '0';
    // data[7] = '2';
    // data[8] = '0';
    // data[9] = '0';
    // data[10] = '0';
    // data[11] = '3';
    array = reinterpret_cast<A*>(data);
    std::cout << array << std::endl;
    array = (A*)data;
    std::cout << array << std::endl;
    std::cout << data << std::endl;

    // std::cout << "array->i: " << array->i << std::endl;
    // std::cout << "array->a[0]: " << array->a[0] << std::endl;

    // std::cout << *array << std::endl;
    // std::cout << *data << std::endl;

    int* i;
    char s[4]{'0'};
    int* heapi = new int;
    char* heapS = new char[4];
    heapS[0] = '0';
    heapS[1] = '0';
    heapS[2] = '1';
    heapS[3] = '1';

    for (int i = 0; i < 4; ++i) {
        std::cout << (int*)&heapS[i] << std::endl;
        std::cout << (int)heapS[i] << std::endl;
    }

    i = (int*)s;
    std::cout << *i << std::endl; 
    heapi = (int*)&heapS[3];
    std::cout << "****** " << *heapi << std::endl;

    memcpy(heapi, heapS, 4);
    std::cout << *heapi << std::endl;

    *heapi = 996;
    memcpy(heapS, heapi, 4);
    std::cout << *(int*)heapS << std::endl;

    for (int i = 0; i < 4; ++i) {
        std::cout << (int*)&heapS[i] << std::endl;
        std::cout << (int)heapS[i] << std::endl;
    }

    delete heapS;


    A* ia;
    char ca[8]{'1', '2', '3', '4', '5', '6', '7', '8'};
    ia = reinterpret_cast<A*>(ca);
    ia->a[0]=9;
    ia->a[1]=5;
    A* ib;
    ib = reinterpret_cast<A*>(ca);
    std::cout << "ib->a[0]: " << ib->a[0] << std::endl;
    std::cout << "ib->a[1]: " << ib->a[1] << std::endl;

    int array1[10] = {1,2,3,4,5,6,7,8,9,10};
    std::move(array1 + 4, array1 + 10, array1 + 3);
    for (int i = 0; i < 10; ++i) {
        std::cout << array1[i] << ",";
    }
    std::cout << std::endl;
    std::move(array1 + 3, array1 + 9, array1 + 4);
    array1[3] = 4;
    for (int i = 0; i < 10; ++i) {
        std::cout << array1[i] << ",";
    }
    std::cout << std::endl;
    return 0;
}

// 808464432
// 808464433
// 808464688
// 808464689
// 825241648
