#include "client/cmd/ConnectCmd.h"
#include <gtest/gtest.h>
#include "Socket_fake.h"
#include "utils/ClassRegister.h"

using namespace EMDB;
using namespace EMDB::ut;
using namespace testing;

TEST(CmdTest, execute_connect_ok)
{
    auto cmd = GET_REGISTER_CLASS(ICmd, COMMAND_CONNECT);
    std::vector<std::string> args{"127.0.0.1", "123456"};
    auto socketFake = std::make_unique<SocketFake>();
    auto socket = std::unique_ptr<Socket>(socketFake.release());
    EXPECT_EQ(cmd->Execute(socket, args), EDB_OK);
}