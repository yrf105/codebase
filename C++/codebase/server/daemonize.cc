#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void daemonize() {
    //  1 创建子进程，关闭父进程
    pid_t pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    //  2 设置文件权限掩码，以获得对新创建文件的完全访问
    umask(0);

    // （可选）打开一个日志文件

    // 3 创建文件会话，设置当前进程 id 为进程组 id
    pid_t sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // 4 切换工作目录
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    // 5 关闭其他已打开的文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // 6 重定向标准输入，标准输出，标准错误，也可使用 dup
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/def/null", O_RDWR);
}


int main(int argc, char** argv) {
    daemon(0, 0);

    return 0;
}