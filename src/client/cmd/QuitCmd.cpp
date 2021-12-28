#include "QuitCmd.h"
#include <iostream>
#include "utils/ClassRegister.h"
#include "ErrorCode.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_QUIT, ICmd, QuitCmd)

int QuitCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string> &args)
{
    if (!socket->IsConnected()) {
        std::cout << ErrCode2Str(EDB_SOCK_NOT_CONNECT) << std::endl;
        return EDB_SOCK_NOT_CONNECT;
    }

    SendOrder(socket, 0);
    // sock.Close();
    return HandleReply();
}

int QuitCmd::HandleReply()
{
    // gQuit = 1;
    return EDB_OK;
}