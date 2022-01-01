#include "QuitCmd.h"
#include <iostream>
#include "utils/ClassRegister.h"
#include "ErrorCode.h"
#include "client/Client.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_QUIT, ICmd, QuitCmd)

int QuitCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string> &args)
{
    if (!socket->IsConnected()) {
        std::cout << ErrCode2Str(EDB_SOCK_NOT_CONNECT) << std::endl;
        return EDB_SOCK_NOT_CONNECT;
    }

    SendOrder(socket, 0);
    socket->Close();
    return HandleReply();
}

int QuitCmd::HandleReply()
{
    Singleton<Client>::Instance().Quit();
    return EDB_OK;
}