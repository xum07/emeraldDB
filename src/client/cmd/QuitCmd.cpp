#include "QuitCmd.h"
#include "client/Client.h"
#include "ErrorCode.h"
#include "pd/Log.h"
#include "utils/ClassRegister.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_QUIT, ICmd, QuitCmd)

int QuitCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string>& args)
{
    if (!socket) {
        EMDB_LOG(W) << "socket is nullptr";
        return EDB_OK;
    }

    if (!socket->IsConnected()) {
        EMDB_LOG(E) << ErrCode2Str(EDB_SOCK_NOT_CONNECT);
        return EDB_SOCK_NOT_CONNECT;
    }

    SendOrder(socket, 0);
//    socket->Close();
    return HandleReply();
}

int QuitCmd::HandleReply()
{
//    Singleton<Client>::Instance().Quit();
    return EDB_OK;
}