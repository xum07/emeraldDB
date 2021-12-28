#ifndef EMDB_SOCKET_FAKE_H
#define EMDB_SOCKET_FAKE_H

#include "oss/Socket.h"

namespace EMDB::ut {
    class SocketFake : public EMDB::Socket {
    public:
        SocketFake() = default;
        ~SocketFake() override = default;
        int InitSocket() override
        {
            return EDB_OK;
        }

        int BindAndListen() override
        {
            return EDB_OK;
        }
        int Accept(int &sock, struct sockaddr *addr, socklen_t *addrlen,
                   std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT) override
        {
            return EDB_OK;
        }
        int Send(const char *msg, int len, std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT, int flags = 0) override
        {
            return EDB_OK;
        }
        int Receive(char *msg, int len, std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT, int flags = 0) override
        {
            return EDB_OK;
        }
        int ReceiveWithoutFlag(char *msg, int len, std::chrono::microseconds timeout = SOCKET_DEFAULT_TIMEOUT) override
        {
            return EDB_OK;
        }
        int Connect() override
        {
            return EDB_OK;
        }
        [[nodiscard]] bool IsConnected() override
        {
            return true;
        }
        void Close() override
        {
            return;
        }

        int SetSocketLinger(int onOff, int linger) override
        {
            return EDB_OK;
        }
        void SetAddress(const std::string &hostName, uint32_t port) override
        {
            return;
        }
        int DisableNagle() override
        {
            return EDB_OK;
        }
        uint32_t GetPeerPort() override
        {
            return 0;
        }
        int GetPeerHost(std::string &host) override
        {
            return EDB_OK;
        }
        uint32_t GetLocalPort() override
        {
            return EDB_OK;
        }
        int GetLocalHost(std::string &host) override
        {
            return EDB_OK;
        }
        int SetTimeout(std::chrono::seconds timeout) override
        {
            return EDB_OK;
        }
        int GetHostName(std::string &name) override
        {
            return EDB_OK;
        }
        uint32_t GetPort(const std::string &serviceName) override
        {
            return 0;
        }
    };
}  // namespace EMDB::ut

#endif  // EMDB_SOCKET_FAKE_H
