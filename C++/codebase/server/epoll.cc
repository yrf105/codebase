#include <arpa/inet.h>
#include <error.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/signal.h>

#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#define LOG_ERROR(str)                                                                       \
    do {                                                                                     \
        std::cerr << __FILE__ << ":" << __LINE__ << " " << errno << " " << str << std::endl; \
        return EXIT_FAILURE;                                                                 \
    } while (0)

int main(int argc, char** argv) {

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>\n";
        return EXIT_FAILURE;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int listenfd = socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    if (listenfd == -1) {
        LOG_ERROR("listen");
    }

    sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(server_addr.sin_addr));
    server_addr.sin_port = htons(port);

    int on = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret == -1) {
        LOG_ERROR("setsockopt");
    }

    ret = bind(listenfd, (sockaddr*)(&server_addr), sizeof(server_addr));
    if (ret == -1) {
        LOG_ERROR("bind");
    }

    ret = listen(listenfd, 5);
    if (ret == -1) {
        LOG_ERROR("listen");
    }

    int epollfd = epoll_create1(EPOLL_CLOEXEC);
    epoll_event event;
    event.data.fd = listenfd;
    event.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

    sockaddr client_addr;
    bzero(&client_addr, sizeof(client_addr));
    socklen_t client_addr_len = sizeof(client_addr);
    int nready = 0;
    std::vector<epoll_event> events(100);

    while (1) {
        nready = epoll_wait(epollfd, events.data(), static_cast<int>(events.size()), -1);

        if (nready < 0) {
            if (errno == EINTR) {
                continue;
            }

            LOG_ERROR("epoll_wait");
        }

        if (nready == 0) {
            continue;
        }

        if (nready == static_cast<int>(events.size())) {
            events.resize(nready * 2);
        }

        for (int i = 0; i < nready; ++i) {
            if (events[i].data.fd == listenfd) {
                if (events[i].events & EPOLLIN) {
                    int connfd = accept4(listenfd, (sockaddr*)(&client_addr), &client_addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
                    if (connfd == -1) {
                        continue;
                    }

                    event.data.fd = connfd;
                    event.events = EPOLLIN;
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &event);
                }
            } else if (events[i].events & EPOLLIN) {
                std::string buffer;
                buffer.resize(1024);
                // char buffer[1024] = {};
                ret = read(events[i].data.fd, buffer.data(), 1023);
                if (ret == -1) {
                    LOG_ERROR("read");
                }
                if (ret == 0) {
                    close(events[i].data.fd);
                    event = events[i];
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &event);
                    
                    std::cout << "客户端关闭连接\n";
                    continue;
                }

                buffer[ret] = '\0';
                std::cout << buffer;
                write(events[i].data.fd, buffer.data(), buffer.size());
            }
        }
    }

    close(listenfd);

    return 0;
}