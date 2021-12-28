#include "client/cmd/ConnectCmd.h"
#include <gtest/gtest.h>
#include "Socket_fake.h"
#include "utils/ClassRegister.h"

using namespace EMDB;
using namespace EMDB::ut;
using namespace testing;

TEST(ConnectCmdTest, execute_ok)
{
    auto cmd = GET_REGISTER_DERIVE_CLASS(ICmd, ConnectCmd, COMMAND_CONNECT);
    std::vector<std::string> args {"", ""};
    auto socketFake = std::make_unique<SocketFake>();
    auto socket = std::unique_ptr<Socket>(socketFake.release());
    EXPECT_EQ(cmd->Execute(socket, args), EDB_OK);
}