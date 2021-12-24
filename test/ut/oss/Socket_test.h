#ifndef EMERALDDB_SOCKET_TEST_H
#define EMERALDDB_SOCKET_TEST_H

#include <gtest/gtest.h>
#include <memory>
#include "oss/Socket.h"

namespace EMDB {
class SocketTest : public ::testing::Test {
public:
    void SetUp() override;
    void TearDown() override;
    
private:
    std::unique_ptr<Socket> _socket;
};
}  // namespace EMDB

#endif  // EMERALDDB_SOCKET_TEST_H