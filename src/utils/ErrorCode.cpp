#include "ErrorCode.h"
#include <unordered_map>

using namespace EMDB;

static std::unordered_map<int, std::string> ERROR_CODE_STRING {
    {EDB_ERR, "default error"},
    {EDB_IO, "io error is occurred"},
    {EDB_INVALIDARG, "invalid argument"},
    {EDB_PERM, "edb_perm"},
    {EDB_OOM, "edb_oom"},
    {EDB_SYS, "system error is occurred"},
    {EDB_PMD_HELP_ONLY, ""},
    {EDB_PMD_FORCE_SYSTEM_EDU, ""},
    {EDB_TIMEOUT, ""},
    {EDB_QUIESCED, "EDB_QUIESCED"},
    {EDB_EDU_INVAL_STATUS, ""},
    {EDB_NETWORK, ""},
    {EDB_NETWORK_CLOSE, "net work is closed"},
    {EDB_APP_FORCED, ""},
    {EDB_IXM_ID_EXIST, "record key is exist"},
    {EDB_HEADER_INVALID, ""},
    {EDB_IXM_ID_NOT_EXIST, "record key not exist"},
    {EDB_NO_ID, "id is needed"},
    {EDB_INVALID_PARAM, "parameter is invalid"},
    {EDB_QUERY_INVALID_ARGUMENT, "invalid query argument"},
    {EDB_INSERT_INVALID_ARGUMENT, "invalid insert argument"},
    {EDB_DELETE_INVALID_ARGUMENT, "invalid delete argument"},
    {EDB_INVALID_RECORD, "invalid record string"},
    {EDB_RECV_DATA_LENGTH_ERROR, ""},
    {EDB_SOCK_INIT_FAILED, "sock init failed"},
    {EDB_SOCK_CONNECT_FAILED, "sock connect remote server failed"},
    {EDB_SOCK_NOT_CONNECT, "sock connection does not exist"},
    {EDB_SOCK_REMOTE_CLOSED, "remote sock connection is closed"},
    {EDB_SOCK_SEND_FAILD, "sock send msg faild"},
    {EDB_MSG_BUILD_FAILED, "msg build failed"},
}

std::string ErrCode2Str(int errCode)
{
    auto itr = ERROR_CODE_STRING.find();
    if (itr == ERROR_CODE_STRING.end()) {
        return std::string();
    }

    return itr->second;
}