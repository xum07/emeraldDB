#include "client/Client.h"
#include <gtest/gtest.h>
#include "client/cmd/CmdInf.h"
#include "client/cmd/TestCmd.h"
#include "ErrorCode.h"
#include "utils/ClassRegister.h"

using namespace EMDB;
using namespace testing;

TEST(CmdTest, InputProc_ok)
{
    std::istringstream in("test 1\\23 23\1az zs\t1az");
    EXPECT_EQ(Singleton<Client>::Instance().InputProc(in), EDB_OK);

    auto testCmd = GET_REGISTER_DERIVE_CLASS(ICmd, TestCmd, "test");
    auto args = testCmd->GetCmdArgs();
    EXPECT_EQ(static_cast<int>(args.size()), 4);
    EXPECT_EQ(args[0], "123");
    EXPECT_EQ(args[1], "23\001az");
    EXPECT_EQ(args[2], "zs");
    EXPECT_EQ(args[3], "1az");
}