#ifndef REACTOR_INETADDRESS_H
#define REACTOR_INETADDRESS_H

#include <netinet/in.h>
#include <string>

namespace tihi {

class InetAddress {
public:
    explicit InetAddress(uint16_t port);
    InetAddress(const std::string& ip, uint16_t port);
    InetAddress(const sockaddr_in& addr_);

    const sockaddr_in& getSockAddrInet() const {
        return addr_;
    }
    void setSockAddrInet(const sockaddr_in& addr) {
        addr_ = addr;
    }

    std::string toHostPort() const;

private:
    sockaddr_in addr_;
};

} // namespace tihi

#endif // REACTOR_INETADDRESS_H