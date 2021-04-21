#include <ucontext.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#define STACKSIZE 4096

static ucontext_t main_ctx, sub_ctx;

struct Args {
    int i;
    std::string str;
};

std::ostream& operator<<(std::ostream& os, const Args& args) {
    return os << args.i << ": " << args.str;
}

static void func(uint32_t low_bits32, uint32_t high_bits32) {
    uintptr_t para_ptr = (((uintptr_t)high_bits32) << 32) | (uintptr_t)low_bits32;
    Args* args = (Args*)(para_ptr);
    std::cout << "hello, sub_ctx -- " << *args << std::endl;
    swapcontext(&sub_ctx, &main_ctx);
    std::cout << "bye, sub_ctx\n";
}

int main(int argc, char** argv) {

    char* sub_ctx_stack = (char*)malloc(sizeof(char) * STACKSIZE);
    stack_t sub_ctx_st;
    sub_ctx_st.ss_size = STACKSIZE;
    sub_ctx_st.ss_sp = sub_ctx_stack;

    getcontext(&sub_ctx);
    sub_ctx.uc_stack = sub_ctx_st;
    sub_ctx.uc_link = &main_ctx;
    Args args{2, "yue"};
    uintptr_t para_ptr = (uintptr_t)(&args);
    // makecontext 构造的协程只能传入
    // (void (*)(void)) 类型的函数指针作为协程的执行对象
    // 所以对于非此类型函数需要强制转换
    // makecontext 只能传递 int 类型长度(32)的参数
    // 所以在64位操作系统上需要分成高低两个部分进行传递
    makecontext(&sub_ctx, (void (*)(void))(func), 2, (uint32_t)para_ptr, (uint32_t)(para_ptr >> 32));

    swapcontext(&main_ctx, &sub_ctx);

    std::cout << "hello, main_ctx\n";

    swapcontext(&main_ctx, &sub_ctx);

    std::cout << "bye, main_ctx\n";

    delete sub_ctx_stack;

    return 0;
}