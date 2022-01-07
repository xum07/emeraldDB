#include "Msg.h"
#include "Log.h"
#include "ErrorCode.h"

using namespace bson;

namespace EMDB {

static int ExpandBuffer(char *&buff, int &realLen, int needLen)
{
    if (needLen <= realLen) {
        EMDB_LOG(I) << "realLen[" << realLen << "] is larger than needLen[" << needLen << "], no need to realloc";
        return EDB_OK;
    }

    if (needLen < 0) {
        EMDB_LOG(E) << "invalid needLen: " << needLen;
        return EDB_INVALIDARG;
    }

    auto oldBuff = buff;
    buff = (char *)realloc(buff, needLen);
    if (!buff) {
        EMDB_LOG(E) << "failed to realloc buffer, size is: " << needLen;
        buff = oldBuff;
        return EDB_OOM;
    }

    realLen = needLen;
    return EDB_OK;
}

int ExtractCheckMsgHeader(MsgHeader &header, int len, enum MsgOperateCode code)
{
    if (header.msgLen < len) {
        EMDB_LOG(E) << "received length[" << header.msgLen << "] less than expect[" << len << "]";
        return EDB_INVALIDARG;
    }

    if (header.operCode != code) {
        EMDB_LOG(E) << "received code[" << header.operCode << "] not equal with expect[" << code << "]";
        return EDB_INVALIDARG;
    }

    return EDB_OK;
}

int BuildReply(char *&buff, int &buffSize, int retCode, bson::BSONObj *retObj)
{
    int size = sizeof(MsgReply);
    if (retObj != nullptr) {
        size += retObj->objsize();
    }

    auto ret = ExpandBuffer(buff, buffSize, size);
    EMDB_LOG_IF(E, ret != EDB_OK) << "Failed to expand buffer for reply";

    auto rep = reinterpret_cast<MsgReply *>(buff);
    rep->header = { size, OperReply };
    rep->retCode = retCode;
    rep->num = 0;
    if (retObj != nullptr) {
        rep->num = 1;
        memcpy(rep->data, retObj->objdata(), retObj->objsize());
    }
    return EDB_OK;
}

int ExtractReply(char *buff, int &retCode, int &numReturn, const char *&objStart)
{
    auto rep = reinterpret_cast<MsgReply *>(buff);
    auto ret = ExtractCheckMsgHeader(rep->header, sizeof(MsgReply), OperReply);
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "failed to check msg header in reply";
        return ret;
    }

    retCode = rep->retCode;
    numReturn = rep->num;
    objStart = (numReturn == 0) ? nullptr : rep->data;
    return EDB_OK;
}

int BuildCommonMsg(char *&buff, int &buffSize, enum MsgOperateCode code, bson::BSONObj &obj)
{
    int size = sizeof(MsgCommon) + obj.objsize();
    auto ret = ExpandBuffer(buff, buffSize, size);
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "Failed to expand buffer for operate: " << code;
        return ret;
    }

    auto msg = reinterpret_cast<MsgCommon *>(buff);
    msg->header = { size, code };
    msg->dataNum = 1;
    memcpy(msg->data, obj.objdata(), obj.objsize());
    return EDB_OK;
}

int BuildBatchCommonMsg(char *&buff, int &buffSize, enum MsgOperateCode code, std::vector<bson::BSONObj *> &obj)
{
    int size = sizeof(MsgCommon);
    for (auto ele : obj) {
        size += ele->objsize();
    }

    auto ret = ExpandBuffer(buff, buffSize, size);
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "Failed to expand buffer for operate: " << code;
        return ret;
    }

    auto msg = reinterpret_cast<MsgCommon *>(buff);
    msg->header = { size, code };
    msg->dataNum = obj.size();
    char *temp = reinterpret_cast<char *>(&msg->data);
    for (auto ele : obj) {
        memcpy(temp, ele->objdata(), ele->objsize());
        temp += ele->objsize();
    }
    return EDB_OK;
}

int ExtractCommonMsg(char *buff, int &num, enum MsgOperateCode code, const char *&objStart)
{
    auto cmd = reinterpret_cast<MsgCommon *>(buff);
    auto ret = ExtractCheckMsgHeader(cmd->header, sizeof(MsgCommon), code);
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "failed to check msg header in operate: " << code;
        return ret;
    }

    num = cmd->dataNum;
    objStart = (num == 0) ? nullptr : cmd->data;
    return EDB_OK;
}

}  // namespace EMDB