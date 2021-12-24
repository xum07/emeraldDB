#include "Socket_test.h"
#include <iostream>

using namespace EMDB;

void SocketTest::SetUp()
{
    _socket = std::make_unique<Socket>();
}

void SocketTest::TearDown()
{
    _socket.reset();
}

TEST_F(SocketTest, test)
{
    std::cout << "Hello, SocketTest" << std::endl;
}