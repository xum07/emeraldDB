#include "TestCmd.h"
#include "pd/Log.h"
#include "utils/ClassRegister.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_TEST, ICmd, TestCmd)

int TestCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string>& args)
{
    EMDB_LOG(I) << "This is testCmd";
    _args.assign(args.begin(), args.end());
    return EDB_OK;
}