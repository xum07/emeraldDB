#include "client/cmd/QuitCmd.h"
#include <gtest/gtest.h>
#include "Socket_fake.h"
#include "utils/ClassRegister.h"

using namespace EMDB;
using namespace EMDB::ut;
using namespace testing;

TEST(QuitCmdTest, execute_ok)
{
    SocketFake socketFake_;
    auto cmd = GET_REGISTER_DERIVE_CLASS(ICmd, QuitCmd, COMMAND_QUIT);
    std::vector<std::string> args {};
    auto socketFake = std::make_unique<SocketFake>();
    auto socket = std::unique_ptr<Socket>(socketFake.release());
    EXPECT_EQ(cmd->Execute(socket, args), EDB_OK);
}