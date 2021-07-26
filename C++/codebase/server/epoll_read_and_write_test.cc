// https://programmer.ink/think/epoll-for-linux-programming.html

// https://www.jianshu.com/p/1c23ad183def

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#define LOG_ERR(message)                                                       \
    do {                                                                       \
        printf("%s:%d errno: %d strerror: %s %s\n", __FILE__, __LINE__, errno, \
               strerror(errno), message);                                      \
        exit(1);                                                               \
    } while (0)

struct Conn {
    std::string readed;
    size_t written;
    bool writeEnable;

    Conn() : readed(), written(0), writeEnable(false) {}
};
std::unordered_map<int, Conn> conns;

void updateEvent(int epollFd, int connFd, uint32_t events, int op) {
    epoll_event event{0};
    event.data.fd = connFd;
    event.events = events;
    int ret = epoll_ctl(epollFd, op, connFd, &event);
    if (ret < 0) {
        LOG_ERR("epoll_ctl error");
    }
}

std::string responseMessage;
void sendResponse(int epollFd, int connFd) {
    std::cout << "发送数据\n";
    if (conns.count(connFd) == 0) {
        return;
    }
    
    Conn& conn = conns[connFd];
    int left = responseMessage.size() - conn.written;
    int n = 0;
    while ((n = send(connFd, responseMessage.data() + conn.written, left, 0)) >
           0) {
        conn.written += n;
        left -= n;
        std::cout << conn.written << " " << left << std::endl;
    }

    if (left == 0) {
        // close(connFd); // keep-alive
        // 服务端不关闭连接，等待客户端关闭连接，然后服务器在 read
        // 事件中关闭连接
        // conns.erase(connFd);

        if (conn.writeEnable) {
            conn.writeEnable = false;
            updateEvent(epollFd, connFd, EPOLLIN, EPOLL_CTL_MOD);
        }
        return;
    }

    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        std::cout << "发送缓冲区已满\n";
        if (!conn.writeEnable) {
            conn.writeEnable = true;
            updateEvent(epollFd, connFd, EPOLLIN | EPOLLOUT, EPOLL_CTL_MOD);
        }
        return;
    }

    if (n <= 0) {
        LOG_ERR("send error");
        close(connFd);
        conns.erase(connFd);
        updateEvent(epollFd, connFd, 0, EPOLL_CTL_DEL);
        return;
    }
}

const std::string kMessageEnd = "\r\n\r\n";
bool parser(const std::string& data) {
    int n = data.size();
    if (n < 4) {
        return false;
    }
    if (data.substr(n - 4, 4) == kMessageEnd) {
        return true;
    }
    return false;
}

const int M = 1048576;

int main(int argc, char** argv) {
    responseMessage =
        "HTTP/1.1 200 OK\r\nContent-Length: 10485760\r\n\r\nHelloWorld";

    for (int i = 0; i < 10485760 - 10; ++i) {
        responseMessage += 'a';
    }

    // SIGPIPE 信号产生的原因：当 socket 收到 RST 包后，
    // 再向 socket 写入数据就会触发 SIGPIPE 信号，
    // 默认会终止程序 详见 http://senlinzhan.github.io/2017/03/02/sigpipe/
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // ip 为 127.0.0.1 时从其他无法访问服务器
    const char* ip = "0.0.0.0";
    int port = 12345;

    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);

    int idleFd = open("/dev/null", O_CLOEXEC | O_RDONLY);

    int listenFd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                          IPPROTO_TCP);
    if (listenFd < 0) {
        LOG_ERR("socket error");
    }
    int on = 1;
    int ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret < 0) {
        LOG_ERR("setsockopot error");
    }
    ret = bind(listenFd, (sockaddr*)(&serverAddr), sizeof(serverAddr));
    if (ret < 0) {
        LOG_ERR("bind error");
    }
    ret = listen(listenFd, 20);
    if (ret < 0) {
        LOG_ERR("listen error");
    }

    int epollFd = epoll_create1(EPOLL_CLOEXEC);
    epoll_event event;
    event.data.fd = listenFd;
    event.events = EPOLLIN;
    // event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, listenFd, &event);

    int nReady = 0;
    std::vector<epoll_event> events(100);
    sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof(clientAddr));
    socklen_t clientAddrLen = sizeof(clientAddr);

    while (true) {
        nReady = epoll_wait(epollFd, events.data(),
                            static_cast<int>(events.size()), -1);

        if (nReady < 0) {
            if (errno == EINTR) {
                continue;
            }
            LOG_ERR("epoll_wait error");
        }

        if (nReady == 0) {
            continue;
        }

        if (nReady == static_cast<int>(events.size())) {
            events.resize(nReady * 2);
        }

        for (int i = 0; i < nReady; ++i) {
            if (events[i].data.fd == listenFd) {
                int connFd =
                    accept4(listenFd, (sockaddr*)(&clientAddr), &clientAddrLen,
                            SOCK_CLOEXEC | SOCK_NONBLOCK);
                if (connFd < 0) {
                    if (errno == EMFILE) {
                        close(idleFd);
                        connFd = accept4(listenFd, (sockaddr*)(&clientAddr),
                                         &clientAddrLen,
                                         SOCK_CLOEXEC | SOCK_NONBLOCK);
                        close(connFd);
                        idleFd = open("/dev/null", O_CLOEXEC | O_RDONLY);
                    }

                    LOG_ERR("accept4 error");
                    continue;
                }
                event.data.fd = connFd;
                event.events = EPOLLIN;
                // event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollFd, EPOLL_CTL_ADD, connFd, &event);

                std::cout << connFd << " 已连接\n";
            } else if ( events[i].events & (EPOLLIN | EPOLLERR)) {  
                // EPOLLERR
                // 当读取端关闭时，也会为管道的写入端报告此事件。
                // https://man7.org/linux/man-pages/man2/epoll_ctl.2.html

                // 可读事件

                size_t bufferSize = 4096;
                std::string buffer;
                buffer.resize(bufferSize);
                int connFd = events[i].data.fd;
                Conn& conn = conns[connFd];
                int n = 0;
                std::cout << connFd << " 可读\n";

                while ((n = ::read(connFd, &buffer[0], bufferSize)) > 0) {
                    std::string& readed = conn.readed;
                    readed.append(buffer, 0, n);
                    // 判断缓冲区中是否接收到一个完整的数据包，
                    // 若已接收到完成的数据包，则处理数据，并执行
                    // sendResponse();
                    std::cout << n << " " << buffer << readed << std::endl;
                    if (parser(readed)) {
                        sendResponse(epollFd, connFd);
                    }
                }

                if (n < 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
                    std::cout << "已经将缓冲区中的数据读完了: \n";
                    continue;
                }

                if (n < 0) {
                    LOG_ERR("read error");
                }

                std::cout << "客户端关闭连接\n";
                close(connFd);
                conns.erase(connFd);
                // Linux 2.6.9 以后第三个参数可以置为 nullptr
                epoll_ctl(epollFd, EPOLL_CTL_DEL, connFd, nullptr);

            } else if (events[i].events & EPOLLOUT) {
                // 可写事件
                int connFd = events[i].data.fd;
                std::cout << connFd << " 可写\n";
                sendResponse(epollFd, connFd);
            }
        }
    }

    return 0;
}