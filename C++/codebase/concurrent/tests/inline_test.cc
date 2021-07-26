#if 1

// 即使加了 -O 选项也不会 inline
// g++ inline_test.cc doSomething.cc -O -o inline_test_noInline
extern void doSomething(); // 定义在 doSomething.cc 中

// objdump -d inline_test_noInline 
// 0000000000001129 <main>:
//     1129:       f3 0f 1e fa             endbr64 
//     112d:       48 83 ec 08             sub    $0x8,%rsp
//     1131:       e8 0a 00 00 00          callq  1140 <_Z11doSomethingv>
//     1136:       b8 00 00 00 00          mov    $0x0,%eax
//     113b:       48 83 c4 08             add    $0x8,%rsp
//     113f:       c3                      retq

#else

// 只有加了 -O 选项才会内联
// g++ inline_test.cc -O -o inline_test_inline
inline void doSomething() {}

// objdump -d inline_test_inline
// 0000000000001129 <main>:
//     1129:       f3 0f 1e fa             endbr64 
//     112d:       b8 00 00 00 00          mov    $0x0,%eax
//     1132:       c3                      retq   
//     1133:       66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)
//     113a:       00 00 00 
//     113d:       0f 1f 00                nopl   (%rax)

#endif
int main() {

    doSomething();

    return 0;
}