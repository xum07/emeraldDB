#ifndef EMERALDDB_COMMAND_INTERFACE_H
#define EMERALDDB_COMMAND_INTERFACE_H

#include <bson/src/util/json.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "ErrorCode.h"
#include "oss/Socket.h"

namespace EMDB {

constexpr int MSG_BUFF_SIZE = 4096;
const std::string COMMAND_QUIT = "quit";
const std::string COMMAND_INSERT = "insert";
const std::string COMMAND_QUERY = "query";
const std::string COMMAND_DELETE = "delete";
const std::string COMMAND_HELP = "help";
const std::string COMMAND_CONNECT = "connect";
const std::string COMMAND_TEST = "test";
const std::string COMMAND_SNAPSHOT = "snapshot";

class ICmd {
    using MsgBuildFunc = std::function<int(char*& buff, int& buffSize, bson::BSONObj& obj)>;

public:
    ICmd() = default;
    virtual ~ICmd() = default;
    virtual int Execute(std::unique_ptr<Socket>& socket, std::vector<std::string>& args)
    {
        return EDB_OK;
    }

protected:
    virtual int HandleReply() { return EDB_OK; }
    int RcvReply(std::unique_ptr<Socket>& socket);
    int RcvProc(std::unique_ptr<Socket>& socket, char* buff, int buffSize);
    int SendOrder(std::unique_ptr<Socket>& socket, MsgBuildFunc& msgBuildFunc);
    int SendOrder(std::unique_ptr<Socket>& socket, int opCode);

protected:
    char _recvBuff[MSG_BUFF_SIZE];
    char _sendBuff[MSG_BUFF_SIZE];
    std::string _json;
};
}  // namespace EMDB

#endif  // EMERALDDB_COMMAND_INTERFACE_H