#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <vector>

#define LOG_ERROR(str)                                                                       \
    do {                                                                                     \
        std::cerr << __FILE__ << ":" << __LINE__ << " " << errno << " " << str << std::endl; \
        return EXIT_FAILURE;                                                                 \
    } while (0)

using PollFdList = std::vector<pollfd>;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>\n";
        return -1;
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);

    int listenfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (listenfd == -1) {
        LOG_ERROR("listenfd");
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
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

    pollfd poll_fd;
    poll_fd.fd = listenfd;
    poll_fd.events = POLLIN;
    poll_fd.revents = 0;

    PollFdList pollfds;
    pollfds.push_back(poll_fd);

    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_addr_len = sizeof(client_addr);

    char buffer[4096];

    while (true) {
        int nready = poll(pollfds.data(), pollfds.size(), -1);
        if (nready < 0) {
            if (errno == EINTR) {
                continue;
            }

            LOG_ERROR("poll");
        }
        if (nready == 0) {
            continue;
        }

        if (pollfds.begin()->revents & POLLIN) {
            // pollfds.begin()->revents &= ~POLLIN;
            --nready;

            int connfd = accept4(listenfd, (sockaddr*)(&client_addr), &client_addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
            if (connfd == -1) {
                if (errno == EMFILE) {
                    close(idlefd);
                    idlefd = accept(listenfd, nullptr, nullptr);
                    close(idlefd);
                    idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
                }

                continue;
            }

            poll_fd.fd = connfd;
            poll_fd.events = POLLIN;
            poll_fd.revents = 0;

            pollfds.push_back(poll_fd);

            if (nready == 0) {
                continue;
            }
        }

        for (auto it = pollfds.begin() + 1; it != pollfds.end() && nready > 0; ++it) {
            if (it->revents & POLLIN) {
                // it->revents &= ~POLLIN;
                --nready;

                ret = read(it->fd, buffer, 4095);

                if (ret == -1) {
                    LOG_ERROR("read");
                }

                if (ret == 0) {
                    std::cout << "客户端断开连接\n";

                    close(it->fd);
                    it = pollfds.erase(it);
                    --it;

                    continue;
                }

                buffer[ret] = '\0';

                std::cout << buffer;

                write(it->fd, buffer, strlen(buffer));
            }
        }
    }

    close(listenfd);

    return 0;
}