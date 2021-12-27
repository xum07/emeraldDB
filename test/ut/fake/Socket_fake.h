#ifndef EMDB_SOCKET_FAKE_H
#define EMDB_SOCKET_FAKE_H

#include <gmock/gmock.h>
#include "oss/Socket.h"

namespace EMDB {
class SocketMock : public Socket {
    MOCK_METHOD0(InitSocket, int());
    MOCK_METHOD0(BindAndListen, int());
    MOCK_METHOD4(Accept, int(int & , struct sockaddr *, socklen_t *, std::chrono::microseconds));
    MOCK_METHOD4(Send, int(const char*, int, std::chrono::microseconds, int));
    MOCK_METHOD4(Receive, int(char * , int, std::chrono::microseconds, int));
    MOCK_METHOD3(ReceiveWithoutFlag, int(char * , int, std::chrono::microseconds));
    MOCK_METHOD0(Connect, int());
    MOCK_METHOD0(IsConnected, bool());
    MOCK_METHOD0(Close, void());

    MOCK_METHOD2(SetSocketLinger, int(int, int));
    MOCK_METHOD2(SetAddress, int(const std::string&, uint32_t));
    MOCK_METHOD0(DisableNagle, int());
    MOCK_METHOD0(GetPeerPort, uint32_t());
    MOCK_METHOD1(GetPeerHost, int(std::string&));
    MOCK_METHOD0(GetLocalPort, uint32_t());
    MOCK_METHOD1(GetLocalHost, int(std::string&));
    MOCK_METHOD1(SetTimeout, int(std::chrono::seconds));
    MOCK_METHOD1(GetHostName, int(std::string&));
    MOCK_METHOD2(GetPort, int(const std::string &, uint16_t &));
};
}  // namespace EMDB

#endif  // EMDB_SOCKET_FAKE_H
