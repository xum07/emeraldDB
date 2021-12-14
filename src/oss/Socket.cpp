#include "Socket.h"
#include <iostream>
#include <netdb.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include "ErrorCode.h"

using namespace EMDB;
using namespace std::chrono_literals;
using namespace std::chrono;

constexpr int LINGER_TURN_ON = 1;
constexpr int MAX_RECEIVE_RETRY_TIMES = 5;

Socket::Socket(uint32_t port, std::chrono::microseconds timeout)
        : _localAddr(), _peerAddr(), _peerAddrLen(sizeof(_peerAddr))
{
    _timeout = timeout;
    _localAddr.sin_family = AF_INET;
    _localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _localAddr.sin_port = htons(port);
    _localAddrLen = sizeof(_localAddr);
}

Socket::Socket(const std::string &hostName, uint32_t port, std::chrono::microseconds timeout)
{
    _timeout = timeout;
    SetAddress(hostName, port);
}

Socket::Socket(int fd, std::chrono::microseconds timeout)
        : _localAddrLen(sizeof(_localAddr)), _peerAddr(), _peerAddrLen(sizeof(_peerAddr))
{
    _fd = fd;
    _init = true;
    _timeout = timeout;
    auto ret = getsockname(_fd, reinterpret_cast<sockaddr *>(&_localAddr), &_localAddrLen);
    if (ret != 0) {
        std::cout << "failed to sock name, error=" << errno << std::endl;
    } else {
        ret = getpeername(_fd, reinterpret_cast<sockaddr *>(&_peerAddr), &_peerAddrLen);
        if (ret != 0) {
            std::cout << "failed to get peer name, error=" << errno << std::endl;
        }
    }
}

int Socket::InitSocket()
{
    if (_init) {
        return EDB_OK;
    }

    _peerAddr = {};
    _peerAddrLen = sizeof(_peerAddr);
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_fd == -1) {
        std::cout << "failed to init socket, error=" << errno << std::endl;
        return EDB_NETWORK;
    }

    _init = true;
    SetTimeout(duration_cast<seconds>(_timeout));
    return EDB_OK;
}

int Socket::SetSocketLinger(int onOff, int linger)
{
    struct linger lg{onOff, linger};
    auto ret = setsockopt(_fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));
    return ret;
}

void Socket::SetAddress(const std::string &hostName, uint32_t port)
{
    _localAddr = {};
    _peerAddr = {};
    _peerAddrLen = sizeof(_peerAddr);
    auto ht = gethostbyname(hostName.data());
    _localAddr.sin_addr.s_addr = (ht != nullptr) ? *(reinterpret_cast<uint32_t *>(ht->h_addr_list[0]))
                                                 : inet_addr(hostName.data());
    _localAddr.sin_family = AF_INET;
    _localAddr.sin_port = htons(port);
    _localAddrLen = sizeof(_localAddr);
}

int Socket::BindAndListen()
{
    int temp = LINGER_TURN_ON;
    // Allows the socket to be bound to an address that is already in use
    // For database shutdown and restart right away, before socket close
    auto ret = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));
    if (ret != 0) {
        std::cout << "failed to setsockopt SO_REUSEADDR, ret=" << ret << ", errno=" << errno << std::endl;
    }

    constexpr int LINGER_TIME = 30;
    ret = SetSocketLinger(LINGER_TURN_ON, LINGER_TIME);
    if (ret != 0) {
        std::cout << "failed to setsockopt SO_LINGER, ret=" << ret << ", errno=" << errno << std::endl;
    }

    ret = bind(_fd, reinterpret_cast<sockaddr *>(&_localAddr), _localAddrLen);
    if (ret != 0) {
        std::cout << "failed to bind socket, ret=" << ret << ", errno=" << errno << std::endl;
        ret = EDB_NETWORK;
        return ret;
    }

    ret = listen(_fd, SOMAXCONN);
    if (ret != 0) {
        std::cout << "failed to listen socket, ret=" << ret << ", errno" << errno << std::endl;
        ret = EDB_NETWORK;
        return ret;
    }

    return EDB_OK;
}

int Socket::Send(const char *msg, int len, std::chrono::microseconds timeout, int flags)
{
    // if we don't expect to receive anything, no need to continue
    if (len == 0) {
        return EDB_OK;
    }

    auto ret = IsSocketReady(timeout);
    if (ret != EDB_OK) {
        std::cout << "socket is not ready, ret=" << ret << std::endl;
        return ret;
    }

    while (len > 0) {
        // If the connection breaks during communication, the SIGPIPE will be sent which result in program exit
        // Thus set MSG_NOSIGNAL flag to avoid SIGPIPE, but the EPIPE error will still be returned
        ret = send(_fd, msg, len, MSG_NOSIGNAL | flags);
        if (ret == -1) {
            std::cout << "failed to send, ret=" << errno << std::endl;
            return EDB_NETWORK;
        }

        len -= ret;
        msg += ret;
    }

    return EDB_OK;
}

bool Socket::IsConnected()
{
    int ret = send(_fd, "", 0, MSG_NOSIGNAL);
    return ret >= 0;
}

int Socket::Receive(char *msg, int len, std::chrono::microseconds timeout, int flags)
{
    // if we don't expect to receive anything, no need to continue
    if (len == 0) {
        return EDB_OK;
    }

    auto ret = IsSocketReady(timeout);
    if (ret != EDB_OK) {
        std::cout << "socket is not ready, ret=" << ret << std::endl;
        return ret;
    }

    int retryTimes = 0;
    while (len > 0) {
        // If the connection breaks during communication, the SIGPIPE will be sent which result in program exit
        // Thus set MSG_NOSIGNAL flag to avoid SIGPIPE, but the EPIPE error will still be returned
        ret = recv(_fd, msg, len, MSG_NOSIGNAL | flags);
        if (ret > 0) {
            if (flags & MSG_PEEK) {
                return ret;
            }
            len -= ret;
            msg += ret;
        } else if (ret == 0) {
            std::cout << "peer unexpected shutdown" << std::endl;
            return EDB_NETWORK_CLOSE;
        } else {
            ret = errno;
            if (((ret == EAGAIN) || (ret == EWOULDBLOCK)) && (_timeout.count() > 0)) {
                // if timeout, message is partial so we should restart
                std::cout << "receive timeout: ret=" << ret << std::endl;
                return EDB_NETWORK;
            }

            if ((ret == EINTR) && (retryTimes < MAX_RECEIVE_RETRY_TIMES)) {
                retryTimes++;
                continue;
            }
            std::cout << "receive failed, ret=" << ret << std::endl;
            return EDB_NETWORK;
        }
    }

    return EDB_OK;
}

int Socket::ReceiveWithoutFlag(char *msg, int len, std::chrono::microseconds timeout)
{
    // if we don't expect to receive anything, no need to continue
    if (len == 0) {
        return EDB_OK;
    }

    auto ret = IsSocketReady(timeout);
    if (ret != EDB_OK) {
        std::cout << "socket is not ready, ret=" << ret << std::endl;
        return ret;
    }

    // If the connection breaks during communication, the SIGPIPE will be sent which result in program exit
    // Thus set MSG_NOSIGNAL flag to avoid SIGPIPE, but the EPIPE error will still be returned
    ret = recv(_fd, msg, len, MSG_NOSIGNAL);
    if (ret == 0) {
        std::cout << "peer unexpected shutdown";
        return EDB_NETWORK_CLOSE;
    } else if (ret < 0) {
        ret = errno;
        if (((ret == EAGAIN) || (ret == EWOULDBLOCK)) && (_timeout.count() > 0)) {
            // if timeout, message is partial so we should restart
            std::cout << "receive timeout: ret=" << ret << std::endl;
            return EDB_NETWORK;
        }

        std::cout << "receive failed, ret=" << ret << std::endl;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

int Socket::Connect()
{
    int ret = connect(_fd, reinterpret_cast<sockaddr *>(&_localAddr), _localAddrLen);
    if (ret != 0) {
        std::cout << "failed to connect, ret=" << errno << std::endl;
        return EDB_NETWORK;
    }

    ret = getsockname(_fd, reinterpret_cast<sockaddr *>(&_localAddr), &_localAddrLen);
    if (ret != 0) {
        std::cout << "failed to get local address, ret=" << ret << std::endl;
        return EDB_NETWORK;
    }

    ret = getpeername(_fd, reinterpret_cast<sockaddr *>(&_peerAddr), &_peerAddrLen);
    if (ret != 0) {
        std::cout << "failed to get peer address, ret=" << ret << std::endl;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

void Socket::Close()
{
    if (_init) {
        (void) close(_fd);
        _init = false;
    }
}

int Socket::Accept(int *sock, struct sockaddr *addr, socklen_t *addrlen, std::chrono::microseconds timeout)
{
    auto ret = IsSocketReady(timeout);
    if (ret != EDB_OK) {
        std::cout << "socket is not ready, ret=" << ret << std::endl;
        return ret;
    }

    *sock = accept(_fd, addr, addrlen);
    if (*sock == -1) {
        std::cout << "failed to accept socket, ret=" << errno << std::endl;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

int Socket::DisableNagle()
{
    int temp = LINGER_TURN_ON;
    auto ret = setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &temp, sizeof(temp));
    if (ret != 0) {
        std::cout << "failed to setsockopt, ret=" << errno << std::endl;
    }

    ret = setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &temp, sizeof(temp));
    if (ret != 0) {
        std::cout << "failed to setsockopt, ret=" << errno << std::endl;
    }

    return EDB_OK;
}

uint32_t Socket::GetPort(sockaddr_in *addr)
{
    return ntohs(addr->sin_port);
}

int Socket::TransAddr2Host(sockaddr_in *addr, char *host, uint32_t hostLen)
{
    hostLen = (hostLen < NI_MAXHOST) ? hostLen : NI_MAXHOST;
    auto ret = getnameinfo(reinterpret_cast<sockaddr *>(addr), sizeof(addr), host,
                           hostLen, nullptr, 0, NI_NUMERICHOST);
    if (ret != 0) {
        std::cout << "failed to getnameinfo, ret=" << errno << std::endl;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

uint32_t Socket::GetLocalPort()
{
    return GetPort(&_localAddr);
}

uint32_t Socket::GetPeerPort()
{
    return GetPort(&_peerAddr);
}

uint32_t Socket::GetLocalHost(char *host, uint32_t hostLen)
{
    return TransAddr2Host(&_localAddr, host, hostLen);
}

uint32_t Socket::GetPeerHost(char *host, uint32_t hostLen)
{
    return TransAddr2Host(&_peerAddr, host, hostLen);
}

int Socket::SetTimeout(std::chrono::seconds timeout)
{
    struct timeval tv{timeout.count(), 0};
    auto ret = setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret) {
        std::cout << "failed to setsockopt, ret=" << errno << std::endl;
    }

    ret = setsockopt(_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    if (ret) {
        std::cout << "failed to setsockopt, ret=" << errno << std::endl;
    }
    return EDB_OK;
}

int Socket::GetHostName(char *name, size_t nameLen)
{
    return gethostname(name, nameLen);
}

void Socket::GetPort(const std::string &serviceName, uint16_t &port)
{
    auto servInfo = getservbyname(serviceName.data(), "tcp");
    port = (servInfo == nullptr) ? atoi(serviceName.data()) : ntohs(servInfo->s_port);
}

int Socket::IsSocketReady(std::chrono::microseconds &timeout)
{
    constexpr int SECOND_TO_MICROSECOND = 1000000;
    struct timeval maxSelectTime;
    maxSelectTime.tv_sec = duration_cast<seconds>(timeout).count() / SECOND_TO_MICROSECOND;
    maxSelectTime.tv_usec = timeout.count() % SECOND_TO_MICROSECOND;

    auto maxFd = _fd;
    fd_set fds;
    // wait loop until the socket is ready
    for (;;) {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);
        auto ret = select(maxFd + 1, &fds, nullptr, nullptr, timeout.count() > 0 ? &maxSelectTime : nullptr);
        // 0 means timeout
        if (ret == 0) {
            return EDB_TIMEOUT;
        }

        // if < 0, means something wrong
        if (ret < 0) {
            ret = errno;
            // if failed due to interrupt, let's continue
            if (ret == EINTR) {
                continue;
            }
            std::cout << "failed to select from socket, ret=" << ret << std::endl;
            return EDB_NETWORK;
        }

        if (FD_ISSET(_fd, &fds)) {
            break;
        }
    }

    return EDB_OK;
}