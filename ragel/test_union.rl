#include <iostream>
#include <string>

%%{
machine test_union;

action add_digit { 
    val = val * 10 + (fc - '0');
}

main := '0x'i xdigit+ | (digit @add_digit)+ | alpha alnum* '\n';


}%%

%%write data;

int main(int argc, char** argv) {

    int cs, val = 0;

    std::string input;
    std::cin >> input;

    char* p = &input[0];
    char* pe = p + input.size() + 1;

    %%write init;
    %%write exec;

    std::cout << val << std::endl;

    return 0;
}