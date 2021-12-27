#include "ConnectCmd.h"
#include "utils/ClassRegister.h"
#include "ErrorCode.h"

using namespace EMDB;

REGISTER_CLASS(COMMAND_CONNECT, ICmd, ConnectCmd)

int ConnectCmd::Execute(Socket &sock, std::vector<std::string> &args)
{
    if (args.empty() || (args.size() < 2)) {
        std::cout << "Invalid param for ConnectCmd" << std::endl;
        return EDB_ERR;
    }

    _address = args[0];
    _port = std::stoul(args[1]);
    sock.Close();
    sock.SetAddress(_address, _port);
    auto ret = sock.InitSocket();
    if (ret != EDB_OK) {
        std::cout << "Failed to init socket, ret = " << ret << std::endl;
        return ret;
    }

    ret = sock.Connect();
    if (ret != EDB_OK) {
        std::cout << "Failed to connect, ret = " << ret << std::endl;
        return ret;
    }
    sock.disableNagle();
    return EDB_OK;
}