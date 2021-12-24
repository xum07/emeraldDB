#ifndef EMERALDDB_SOCKET_H
#define EMERALDDB_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <chrono>

namespace EMDB {

#define ZERO_TIMEOUT static_cast<std::chrono::microseconds>(0)
#define SOCKET_DEFAULT_TIMEOUT static_cast<std::chrono::microseconds>(10000)

class Socket {
public:
    Socket() : _localAddr(), _localAddrLen(sizeof(_localAddr)), _peerAddr(), _peerAddrLen(sizeof(_peerAddr)) {};
    // create a listening socket
    explicit Socket(uint32_t port, std::chrono::microseconds timeout = ZERO_TIMEOUT);
    // create a connecting socket
    Socket(const std::string &hostName, uint32_t port, std::chrono::microseconds timeout = ZERO_TIMEOUT);
    // create from an existing socket
    explicit Socket(int fd, std::chrono::microseconds timeout = ZERO_TIMEOUT);
    ~Socket() { Close(); }

    int InitSocket();
    int BindAndListen();
    int Accept(int &sock, struct sockaddr *addr, socklen_t *addrlen,
               std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT);
    int Send(const char* msg, int len, std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT, int flags = 0);
    int Receive(char *msg, int len, std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT, int flags = 0);
    int ReceiveWithoutFlag(char *msg, int len, std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT);
    int Connect();
    [[nodiscard]] bool IsConnected() const;
    void Close();

    int SetSocketLinger(int onOff, int linger) const;
    void SetAddress(const std::string &hostName, uint32_t port);
    int DisableNagle();
    uint32_t GetPeerPort();
    uint32_t GetPeerHost(std::string& host);
    uint32_t GetLocalPort();
    uint32_t GetLocalHost(std::string& host);
    int SetTimeout(std::chrono::seconds timeout);
    static int GetHostName(std::string& name);
    static void GetPort(const std::string &serviceName, uint16_t &port);

protected:
    uint32_t GetPort(sockaddr_in *addr);
    int TransAddr2Host(sockaddr_in *addr, std::string& host);

private:
    int IsSocketReady(std::chrono::microseconds &timeout);

private:
    int _fd{0};
    struct sockaddr_in _localAddr;
    socklen_t _localAddrLen;
    struct sockaddr_in _peerAddr;
    socklen_t _peerAddrLen;
    bool _init{false};
    std::chrono::microseconds _timeout{ZERO_TIMEOUT};
};
}  // namespace EMDB

#endif  // EMERALDDB_SOCKET_H
