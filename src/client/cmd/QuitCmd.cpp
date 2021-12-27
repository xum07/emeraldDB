#include "QuitCmd.h"
#include "utils/ClassRegister.h"
#include "ErrorCode.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_QUIT, ICmd, QuitCmd)

int QuitCmd::Execute(Socket &sock, std::vector<std::string> &args)
{
    if (!sock.isConnected()) {
        return ErrCode2Str(EDB_SOCK_NOT_CONNECT);
    }

    auto ret = sendOrder(sock, 0);
    // sock.close();
    ret = handleReply();
    return ret;
}

int QuitCmd::HandleReply()
{
    // gQuit = 1;
    return EDB_OK;
}