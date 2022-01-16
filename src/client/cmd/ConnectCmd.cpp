#include "ConnectCmd.h"
#include "ErrorCode.h"
#include "pd/Log.h"
#include "utils/ClassRegister.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_CONNECT, ICmd, ConnectCmd)

int ConnectCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string>& args)
{
    if (args.size() < 2) {
        EMDB_LOG(E) << "Invalid param for ConnectCmd";
        return EDB_INVALID_PARAM;
    }

    if (socket) {
        EMDB_LOG(W) << "close current socket and reopen it";
        socket->Close();
    }

    // socket is not the aim of this project, thus args check will be ignored
    // not only here, but also lots of codes will design based on the thought
    _address = args[0];
    _port = std::stoul(args[1]);
    socket->SetAddress(_address, _port);
    EMDB_LOG(I) << "connect address(" << _address << "):port(" << _port << ")";
    auto ret = socket->InitSocket();
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "Failed to init socket, ret=" << ret;
        return ret;
    }

    ret = socket->Connect();
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "Failed to connect, ret=" << ret;
        return ret;
    }
    socket->DisableNagle();
    return EDB_OK;
}