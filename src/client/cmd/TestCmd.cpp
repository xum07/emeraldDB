#include "TestCmd.h"
#include <iostream>
#include "utils/ClassRegister.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_TEST, ICmd, TestCmd)

int TestCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string> &args)
{
    std::cout << "This is testCmd" << std::endl;
    _args.assign(args.begin(), args.end());
    return EDB_OK;
}