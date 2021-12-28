#include "ConnectCmd.h"
#include "utils/ClassRegister.h"
#include "ErrorCode.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_CONNECT, ICmd, ConnectCmd)

int ConnectCmd::Execute(std::unique_ptr<Socket>& socket, std::vector<std::string> &args)
{
    if (args.size() < 2) {
        std::cout << "Invalid param for ConnectCmd" << std::endl;
        return EDB_INVALID_PARAM;
    }

    // socket is not the aim of this project, thus args check will be ignored
    // not only here, but also lots of codes will design based on the thought
    _address = args[0];
    _port = std::stoul(args[1]);
    socket->Close();
    socket->SetAddress(_address, _port);
    auto ret = socket->InitSocket();
    if (ret != EDB_OK) {
        std::cout << "Failed to init socket, ret = " << ret << std::endl;
        return ret;
    }

    ret = socket->Connect();
    if (ret != EDB_OK) {
        std::cout << "Failed to connect, ret = " << ret << std::endl;
        return ret;
    }
    socket->DisableNagle();
    return EDB_OK;
}