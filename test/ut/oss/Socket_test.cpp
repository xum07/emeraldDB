#include "oss/Socket.h"
#include <gtest/gtest.h>
#include "ErrorCode.h"

using namespace EMDB;
using namespace std::chrono_literals;

constexpr uint32_t LOCAL_PORT_TEST = 54321;

TEST(SocketTest, create_listening_socket_correct)
{
    auto socket = std::make_unique<Socket>(LOCAL_PORT_TEST, 3s);
    auto localPort = socket->GetLocalPort();
    auto peerPort = socket->GetPeerPort();
    EXPECT_EQ(localPort, LOCAL_PORT_TEST);
    EXPECT_EQ(peerPort, static_cast<uint32_t>(0));
}

TEST(SocketTest, create_conning_socket_correct)
{
    auto socket = std::make_unique<Socket>("localhost", LOCAL_PORT_TEST);
    auto localPort = socket->GetLocalPort();
    EXPECT_EQ(localPort, LOCAL_PORT_TEST);

    std::string localHost;
    EXPECT_EQ(socket->GetLocalHost(localHost), EDB_OK);
    EXPECT_EQ(localHost, "127.0.0.1");
}
