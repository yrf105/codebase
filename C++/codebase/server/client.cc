#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

#define LOG_ERROR (std::cerr << __FILE__ << ":" << __LINE__ << " " << errno << ":" << strerror(errno) << " ")

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>\n";
        return -1;
    }
    
    const char* ip = argv[1];
    int port =  atoi(argv[2]);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(server_addr.sin_addr));
    server_addr.sin_port = htonl(port);

    int connfd = socket(PF_INET, SOCK_STREAM, 0);
    if (connfd == -1) {
        LOG_ERROR << "socket\n";
        return -1;
    }

    int ret = connect(connfd, (sockaddr*)(&server_addr), sizeof(server_addr));
    if (ret == -1) {
        LOG_ERROR << "connect\n";
        return -1;
    }

    char buffer[1024] = "hello, world";
    write(connfd, buffer, strlen(buffer));
    ret = read(connfd, buffer, 1023);
    buffer[ret] = '\0';
    std::cout << buffer << std::endl;

    close(connfd);

    return 0;
}