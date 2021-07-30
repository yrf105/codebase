#include "InetAddress.h"

#include "SocketsOps.h"

namespace tihi {

InetAddress::InetAddress(uint16_t port) : addr_{0} {
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = sockets::hostToNetWork32(INADDR_ANY);
    addr_.sin_port = sockets::hostToNetWork16(port);
}

InetAddress::InetAddress(const std::string& ip, uint16_t port) : addr_{0} {
    sockets::fromHostPort(ip.c_str(), port, addr_);
}

InetAddress::InetAddress(const sockaddr_in& addr) : addr_(addr) {

}

std::string InetAddress::toHostPort() const {
    char buf[32];
    sockets::toHostPort(buf, sizeof(buf), addr_);
    return buf;
}


} // namespace tihi