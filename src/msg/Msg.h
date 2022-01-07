#ifndef EMERALDDB_MESSAGE_H
#define EMERALDDB_MESSAGE_H

#include <climits>
#include <vector>
#include "bson.h"

#define RETURN_CODE_STATE_OK 1

namespace EMDB {

enum MsgOperateCode : int {
    OperReply = 1,
    OperInsert,
    OperDelete,
    OperQuery,
    OperCommand,
    OperDisconnect,
    OperConnect,
    OperSnapshot,
    OperInvalid = INT_MAX,
};

struct MsgHeader {
    int msgLen = 0;
    int operCode = OperInvalid;
};

struct MsgCommon {
    MsgHeader header;
    int dataNum = 0;  // data数组中的元素个数
    char data[];
};

struct MsgReply {
    MsgHeader header;
    int retCode = 0;
    int num = 0;
    int dataLen = 0;
    char data[];
};

int BuildReply(char *&buff, int &buffSize, int retCode, bson::BSONObj *retObj);

int ExtractReply(char *buff, int &retCode, int &numReturn, const char *&objStart);

int BuildCommonMsg(char *&buff, int &buffSize, enum MsgOperateCode code, bson::BSONObj &obj);

int BuildBatchCommonMsg(char *&buff, int &buffSize, enum MsgOperateCode code, std::vector<bson::BSONObj *> &obj);

int ExtractCommonMsg(char *buff, int &num, enum MsgOperateCode code, const char *&objStart);

}  // namespace EMDB

#endif  // EMERALDDB_MESSAGE_H
