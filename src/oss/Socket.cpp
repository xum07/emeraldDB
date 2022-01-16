#include "Socket.h"
#include <netdb.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <memory>
#include "ErrorCode.h"
#include "pd/Log.h"

using namespace EMDB;
using namespace std::chrono_literals;
using namespace std::chrono;

constexpr int LINGER_TURN_ON = 1;
constexpr int MAX_RECEIVE_RETRY_TIMES = 5;

Socket::Socket(uint32_t port, std::chrono::microseconds timeout)
{
    _timeout = timeout;
    _localAddr.sin_family = AF_INET;
    _localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _localAddr.sin_port = htons(port);
    _localAddrLen = sizeof(_localAddr);
}

Socket::Socket(const std::string& hostName, uint32_t port, std::chrono::microseconds timeout)
{
    _timeout = timeout;
    SetAddress(hostName, port);
}

Socket::Socket(int fd, std::chrono::microseconds timeout)
{
    _fd = fd;
    _init = true;
    _timeout = timeout;
    errno = 0;
    auto ret = getsockname(_fd, reinterpret_cast<sockaddr*>(&_localAddr), &_localAddrLen);
    if (ret != 0) {
        EMDB_LOG(E) << "failed to sock name, error=" << errno;
        _init = false;
    } else {
        ret = getpeername(_fd, reinterpret_cast<sockaddr*>(&_peerAddr), &_peerAddrLen);
        EMDB_LOG_IF(E, ret != 0) << "failed to get peer name, error=" << errno;
    }
}

int Socket::InitSocket()
{
    if (_init) {
        return EDB_OK;
    }

    errno = 0;
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_fd == -1) {
        EMDB_LOG(E) << "failed to init socket, error=" << errno;
        return EDB_NETWORK;
    }

    _init = true;
    SetTimeout(duration_cast<seconds>(_timeout));
    return EDB_OK;
}

int Socket::SetSocketLinger(int onOff, int linger)
{
    struct linger lg {
        onOff, linger
    };
    auto ret = setsockopt(_fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));
    return ret;
}

void Socket::SetAddress(const std::string& hostName, uint32_t port)
{
    _localAddr = {};
    _peerAddr = {};
    _peerAddrLen = sizeof(_peerAddr);
    auto ht = gethostbyname(hostName.data());
    _localAddr.sin_addr.s_addr = (ht != nullptr)
                                     ? *(reinterpret_cast<uint32_t*>(ht->h_addr_list[0]))
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
    errno = 0;
    auto ret = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));
    EMDB_LOG_IF(E, ret != 0)
        << "failed to setsockopt SO_REUSEADDR, ret=" << ret << ", errno=" << errno;

    constexpr int LINGER_TIME = 30;
    ret = SetSocketLinger(LINGER_TURN_ON, LINGER_TIME);
    EMDB_LOG_IF(E, ret != 0)
        << "failed to setsockopt SO_LINGER, ret=" << ret << ", errno=" << errno;

    ret = bind(_fd, reinterpret_cast<sockaddr*>(&_localAddr), _localAddrLen);
    if (ret != 0) {
        EMDB_LOG(E) << "failed to bind socket, ret=" << ret << ", errno=" << errno;
        return EDB_NETWORK;
    }

    ret = listen(_fd, SOMAXCONN);
    if (ret != 0) {
        EMDB_LOG(E) << "failed to listen socket, ret=" << ret << ", errno=" << errno;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

int Socket::Send(const char* msg, int len, std::chrono::microseconds timeout, int flags)
{
    // if we don't expect to receive anything, no need to continue
    if (len == 0) {
        return EDB_OK;
    }

    auto ret = IsSocketReady(timeout, false);
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "socket is not ready";
        return ret;
    }

    while (len > 0) {
        // If the connection breaks during communication, the SIGPIPE will be sent which result in program exit
        // Thus set MSG_NOSIGNAL flag to avoid SIGPIPE, but the EPIPE error will still be returned
        ret = send(_fd, msg, len, MSG_NOSIGNAL | flags);
        if (ret == -1) {
            EMDB_LOG(E) << "failed to send, ret=" << ret;
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

int Socket::Receive(char* msg, int len, std::chrono::microseconds timeout, int flags)
{
    // if we don't expect to receive anything, no need to continue
    if (len == 0) {
        return EDB_OK;
    }

    auto ret = IsSocketReady(timeout, true);
    if (ret != EDB_OK) {
        return ret;
    }

    int retryTimes = 0;
    while (len > 0) {
        // If the connection breaks during communication, the SIGPIPE will be sent which result in program exit
        // Thus set MSG_NOSIGNAL flag to avoid SIGPIPE, but the EPIPE error will still be returned
        errno = 0;
        ret = recv(_fd, msg, len, MSG_NOSIGNAL | flags);
        if (ret > 0) {
            if (flags & MSG_PEEK) {
                return ret;
            }
            len -= ret;
            msg += ret;
        } else if (ret == 0) {
            EMDB_LOG(E) << "peer unexpected shutdown";
            return EDB_NETWORK_CLOSE;
        } else {
            ret = errno;
            if ((ret == EAGAIN) && (_timeout.count() > 0)) {
                // if timeout, message is partial so we should restart
                EMDB_LOG(E) << "receive timeout: ret=" << ret;
                return EDB_NETWORK;
            }

            if ((ret == EINTR) && (retryTimes < MAX_RECEIVE_RETRY_TIMES)) {
                retryTimes++;
                continue;
            }
            EMDB_LOG(E) << "receive failed, ret=" << ret;
            return EDB_NETWORK;
        }
    }

    return EDB_OK;
}

int Socket::ReceiveWithoutFlag(char* msg, int len, std::chrono::microseconds timeout)
{
    // if we don't expect to receive anything, no need to continue
    if (len == 0) {
        return EDB_OK;
    }

    auto ret = IsSocketReady(timeout, true);
    if (ret != EDB_OK) {
        return ret;
    }

    // If the connection breaks during communication, the SIGPIPE will be sent which result in program exit
    // Thus set MSG_NOSIGNAL flag to avoid SIGPIPE, but the EPIPE error will still be returned
    errno = 0;
    ret = recv(_fd, msg, len, MSG_NOSIGNAL);
    if (ret == 0) {
        EMDB_LOG(E) << "peer unexpected shutdown";
        return EDB_NETWORK_CLOSE;
    } else if (ret < 0) {
        ret = errno;
        if ((ret == EAGAIN) && (_timeout.count() > 0)) {
            // if timeout, message is partial so we should restart
            EMDB_LOG(E) << "receive timeout: ret=" << ret;
            return EDB_NETWORK;
        }

        EMDB_LOG(E) << "receive failed: ret=" << ret;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

int Socket::Connect()
{
    errno = 0;
    int ret = connect(_fd, reinterpret_cast<sockaddr*>(&_localAddr), _localAddrLen);
    if (ret != 0) {
        EMDB_LOG(E) << "failed to connect, ret=" << ret << ", errno=" << errno;
        return EDB_NETWORK;
    }

    ret = getsockname(_fd, reinterpret_cast<sockaddr*>(&_localAddr), &_localAddrLen);
    if (ret != 0) {
        EMDB_LOG(E) << "failed to get local address, ret=" << ret;
        return EDB_NETWORK;
    }

    ret = getpeername(_fd, reinterpret_cast<sockaddr*>(&_peerAddr), &_peerAddrLen);
    if (ret != 0) {
        EMDB_LOG(E) << "failed to get peer address, ret=" << ret;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

void Socket::Close()
{
    if (_init) {
        (void)close(_fd);
        _init = false;
    }
}

int Socket::Accept(int& sock, struct sockaddr* addr, socklen_t* addrlen,
                   std::chrono::microseconds timeout)
{
    auto ret = IsSocketReady(timeout, true);
    if (ret != EDB_OK) {
        return ret;
    }

    errno = 0;
    sock = accept(_fd, addr, addrlen);
    if (sock == -1) {
        EMDB_LOG(E) << "failed to accept socket, ret=" << ret << ", errno=" << errno;
        return EDB_NETWORK;
    }

    return EDB_OK;
}

int Socket::DisableNagle()
{
    errno = 0;
    int temp = LINGER_TURN_ON;
    auto ret = setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &temp, sizeof(temp));
    EMDB_LOG_IF(E, ret != 0) << "failed to setsockopt, ret=" << ret << ", errno=" << errno;

    ret = setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &temp, sizeof(temp));
    EMDB_LOG_IF(E, ret != 0) << "failed to setsockopt, ret=" << ret << ", errno=" << errno;

    return EDB_OK;
}

uint32_t Socket::GetPort(sockaddr_in* addr) { return ntohs(addr->sin_port); }

int Socket::TransAddr2Host(sockaddr_in* addr, std::string& host)
{
    auto tmpHost = std::make_unique<char[]>(NI_MAXHOST);
    if (!tmpHost) {
        EMDB_LOG(E) << "failed to malloc, size:" << NI_MAXHOST;
        return EDB_ERR;
    }

    errno = 0;
    auto ret = getnameinfo(reinterpret_cast<sockaddr*>(addr), sizeof(sockaddr), tmpHost.get(),
                           NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
    if (ret != 0) {
        EMDB_LOG(E) << "failed to getnameinfo, ret=" << ret << ", errno=" << errno;
        return EDB_NETWORK;
    }

    host = std::string(tmpHost.get());
    return EDB_OK;
}

uint32_t Socket::GetLocalPort() { return GetPort(&_localAddr); }

uint32_t Socket::GetPeerPort() { return GetPort(&_peerAddr); }

int Socket::GetLocalHost(std::string& host) { return TransAddr2Host(&_localAddr, host); }

int Socket::GetPeerHost(std::string& host) { return TransAddr2Host(&_peerAddr, host); }

int Socket::SetTimeout(std::chrono::seconds timeout)
{
    struct timeval tv {
        timeout.count(), 0
    };

    errno = 0;
    auto ret = setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    EMDB_LOG_IF(E, ret != 0) << "failed to setsockopt, ret=" << ret << ", errno=" << errno;

    ret = setsockopt(_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    EMDB_LOG_IF(E, ret != 0) << "failed to setsockopt, ret=" << ret << ", errno=" << errno;
    return EDB_OK;
}

int Socket::GetHostName(std::string& name)
{
    char tmpName[HOST_NAME_MAX] = {0};
    auto ret = gethostname(tmpName, sizeof(tmpName));
    if (ret != 0) {
        EMDB_LOG(E) << "failed to gethostname";
        return ret;
    }

    name = std::string(tmpName);
    return 0;
}

uint32_t Socket::GetPort(const std::string& serviceName)
{
    auto servInfo = getservbyname(serviceName.data(), "tcp");
    auto port = (servInfo == nullptr) ? std::stoul(serviceName) : ntohs(servInfo->s_port);
    return port;
}

int Socket::IsSocketReady(std::chrono::microseconds& timeout, bool readFromSocket)
{
    constexpr int SECOND_TO_MICROSECOND = 1000000;
    struct timeval maxSelectTime {};
    maxSelectTime.tv_sec = duration_cast<seconds>(timeout).count();
    maxSelectTime.tv_usec = timeout.count() % SECOND_TO_MICROSECOND;

    auto maxFd = _fd;
    fd_set fds;
    // wait loop until the socket is ready
    for (;;) {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);
        errno = 0;
        auto ret = readFromSocket ? select(maxFd + 1, &fds, nullptr, nullptr,
                                           timeout.count() > 0 ? &maxSelectTime : nullptr)
                                  : select(maxFd + 1, nullptr, &fds, nullptr,
                                           timeout.count() > 0 ? &maxSelectTime : nullptr);
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
            EMDB_LOG(E) << "failed to select from socket, ret=" << ret;
            return EDB_NETWORK;
        }

        if (FD_ISSET(_fd, &fds)) {
            break;
        }
    }

    return EDB_OK;
}