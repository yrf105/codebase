#ifndef REACTOR_SOCKETSOPS_H
#define REACTOR_SOCKETSOPS_H

#include <arpa/inet.h>
#include <endian.h>

namespace tihi {
namespace sockets {

inline uint64_t hostToNetWork64(uint64_t host64) {
    return htobe64(host64);
}

inline uint32_t hostToNetWork32(uint32_t host32) {
    return ::htonl(host32);
}

inline uint16_t hostToNetWork16(uint16_t host16) {
    return ::htons(host16);
}

inline uint64_t netWorkToHost64(uint64_t net64) {
    return be64toh(net64);
}

inline uint32_t netWorkToHost32(uint32_t net32) {
    return ::ntohl(net32);
}

inline uint16_t netWorkToHost16(uint16_t net16) {
    return ::ntohs(net16);
}

int createNonblocking();
void bind(int listenfd, const sockaddr_in* addr);
void listen(int listenfd);
int accpet(int listenfd, sockaddr_in* addr);
void close(int sockfd);

void toHostPort(char* buf, size_t size, const sockaddr_in& addr);
void fromHostPort(const char* ip, uint16_t port, sockaddr_in& addr);

sockaddr_in getLocalAddr(int fd);
int getSocketError(int fd);

}  // namespace sockets
}  // namespace tihi

#endif  // REACTOR_SOCKETOPS_H