#ifndef EMERALDDB_COMMAND_INTERFACE_H
#define EMERALDDB_COMMAND_INTERFACE_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <bson/src/util/json.h>
#include "ErrorCode.h"
#include "oss/Socket.h"

namespace EMDB {

const std::string COMMAND_QUIT = "quit";
const std::string COMMAND_INSERT = "insert";
const std::string COMMAND_QUERY = "query";
const std::string COMMAND_DELETE = "delete";
const std::string COMMAND_HELP = "help";
const std::string COMMAND_CONNECT = "connect";
const std::string COMMAND_TEST = "test";
const std::string COMMAND_SNAPSHOT = "snapshot";

class ICmd {
    using MsgBuildFunc = std::function<int(char** buff, int& buffSize, bson::BSONObj& obj)>;
public:
    virtual int Execute(Socket &sock, std::vector<std::string> &args) { return EDB_OK; }

protected:
    int RcvReply(Socket &sock);
    int SendOrder(Socket &sock, MsgBuildFunc &msgBuildFunc);
    int SendOrder(Socket &sock, int opCode);
    virtual int HandleReply() { return EDB_OK; }

protected:
    std::unique_ptr<char []> _recvBuf;
    std::unique_ptr<char []> _sendBuf;
    std::string _json;
};
}  // namespace EMDB

#endif  // EMERALDDB_COMMAND_INTERFACE_H