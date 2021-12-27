#ifndef EMERALDDB_ERRORCODE_H
#define EMERALDDB_ERRORCODE_H

#include <climits>
#include <string>

namespace EMDB {
#define EDB_ERR                              INT_MIN
#define EDB_OK                                0
#define EDB_IO                               -1
#define EDB_INVALIDARG                       -2
#define EDB_PERM                             -3
#define EDB_OOM                              -4
#define EDB_SYS                              -5
#define EDB_PMD_HELP_ONLY                    -6
#define EDB_PMD_FORCE_SYSTEM_EDU             -7
#define EDB_TIMEOUT                          -8
#define EDB_QUIESCED                         -9
#define EDB_EDU_INVAL_STATUS                 -10
#define EDB_NETWORK                          -11
#define EDB_NETWORK_CLOSE                    -12
#define EDB_APP_FORCED                       -13
#define EDB_IXM_ID_EXIST                     -14
#define EDB_HEADER_INVALID                   -15
#define EDB_IXM_ID_NOT_EXIST                 -16
#define EDB_NO_ID                            -17
#define EDB_INVALID_PARAM                    -18
#define EDB_QUERY_INVALID_ARGUMENT           -101
#define EDB_INSERT_INVALID_ARGUMENT          -102
#define EDB_DELETE_INVALID_ARGUMENT          -103
#define EDB_INVALID_RECORD                   -104
#define EDB_RECV_DATA_LENGTH_ERROR           -107
#define EDB_SOCK_INIT_FAILED                 -113
#define EDB_SOCK_CONNECT_FAILED              -114
#define EDB_SOCK_NOT_CONNECT                 -115
#define EDB_SOCK_REMOTE_CLOSED               -116
#define EDB_SOCK_SEND_FAILD                  -117
#define EDB_MSG_BUILD_FAILED                 -119

/**
 * @brief translate an errorCode to string
 * @param errCode the integer of an errorCode
 * @return the meaning of errorCode expressed by string
 */
std::string ErrCode2Str(int errCode);
}  // namespace EMDB

#endif  // EMERALDDB_ERRORCODE_H
