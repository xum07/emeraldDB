#ifndef EMERALDDB_SOCKET_H
#define EMERALDDB_SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <chrono>
#include <string>

namespace EMDB {

#define ZERO_TIMEOUT static_cast<std::chrono::microseconds>(0)
#define SOCKET_DEFAULT_TIMEOUT static_cast<std::chrono::microseconds>(10000)

class Socket {
public:
    Socket() = default;
    // create a listening socket
    explicit Socket(uint32_t port, std::chrono::microseconds timeout = ZERO_TIMEOUT);
    // create a connecting socket
    Socket(const std::string& hostName, uint32_t port,
           std::chrono::microseconds timeout = ZERO_TIMEOUT);
    // create from an existing socket
    explicit Socket(int fd, std::chrono::microseconds timeout = ZERO_TIMEOUT);

    virtual ~Socket() { Close(); }

    virtual int InitSocket();
    virtual int BindAndListen();
    virtual int Accept(int& sock, struct sockaddr* addr, socklen_t* addrlen,
                       std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT);
    virtual int Send(const char* msg, int len,
                     std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT, int flags = 0);
    virtual int Receive(char* msg, int len,
                        std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT, int flags = 0);
    virtual int ReceiveWithoutFlag(char* msg, int len,
                                   std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT);
    virtual int Connect();
    [[nodiscard]] virtual bool IsConnected();
    virtual void Close();
    virtual int SetSocketLinger(int onOff, int linger);
    virtual void SetAddress(const std::string& hostName, uint32_t port);
    virtual int DisableNagle();
    virtual uint32_t GetPeerPort();
    virtual int GetPeerHost(std::string& host);
    virtual uint32_t GetLocalPort();
    virtual int GetLocalHost(std::string& host);
    virtual int SetTimeout(std::chrono::seconds timeout);
    virtual int GetHostName(std::string& name);
    virtual uint32_t GetPort(const std::string& serviceName);

protected:
    uint32_t GetPort(sockaddr_in* addr);
    int TransAddr2Host(sockaddr_in* addr, std::string& host);

private:
    int IsSocketReady(std::chrono::microseconds& timeout);

private:
    int _fd{0};
    struct sockaddr_in _localAddr {};
    socklen_t _localAddrLen{sizeof(_localAddr)};
    struct sockaddr_in _peerAddr {};
    socklen_t _peerAddrLen{sizeof(_peerAddr)};
    bool _init{false};
    std::chrono::microseconds _timeout{ZERO_TIMEOUT};
};
}  // namespace EMDB

#endif  // EMERALDDB_SOCKET_H
