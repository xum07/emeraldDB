#include "HelpCmd.h"
#include <iostream>
#include "ErrorCode.h"
#include "utils/ClassRegister.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_HELP, ICmd, HelpCmd)

int HelpCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string>& args)
{
    std::cout << "List of classes of commands:\n\n";
    std::cout << COMMAND_CONNECT << " [server] [port]-- connecting emeralddb server\n";
    std::cout << COMMAND_INSERT << " -- sending a insert command to emeralddb server\n";
    std::cout << COMMAND_QUERY << " -- sending a query command to emeralddb server\n";
    std::cout << COMMAND_DELETE << " -- sending a delete command to emeralddb server\n";
    std::cout << COMMAND_TEST << " [number]-- sending a test command to emeralddb server\n";
    std::cout << COMMAND_SNAPSHOT << " -- providing current number of record inserting\n";
    std::cout << COMMAND_QUIT << " -- quitting command\n\n";
    std::cout << "Type \"help\" command for help\n" << std::endl;
    return EDB_OK;
}