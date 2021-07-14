#include <iostream>

void foo(int n) {
    int a[n][n];
    a[0][0] = 1;
    std::cout << a[0][0] << std::endl;
}

template <typename T>
class Array2D {
public:
    Array2D(int dim1, int dim2);

private:
    struct Array1D {
        T& operator[](int idx) const;
    };

public:
    Array1D& operator[](int idx) const;
private:
    //...
};

class String {
public:

private:
};

int main(int argc, char** argv) {
    int n;
    std::cin >> n;
    int a[n] = {1, 2};
    a[2] = 1;
    std::cout << a[2] << std::endl;

    // int *data = new int[5][5];

    // Array2D<int> a2(2, 3);
    // a2[1][1];

    return 0;
}