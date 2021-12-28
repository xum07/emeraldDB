#include "CmdInf.h"
#include "ErrorCode.h"

using namespace EMDB;

// msg buff structure: 4bit + others
// the 4bit header is used to store msg length
constexpr int MSG_LENGTH_OCCUPY = sizeof(int);

int ICmd::RcvReply(std::unique_ptr<Socket>& socket)
{
    memset(_recvBuff, 0, MSG_BUFF_SIZE);
    if (!socket->IsConnected()) {
        std::cout << ErrCode2Str(EDB_SOCK_NOT_CONNECT) << std::endl;
        return EDB_SOCK_NOT_CONNECT;
    }

    // first receive the length of the data
    auto ret = RcvProc(socket, _recvBuff, MSG_LENGTH_OCCUPY);
    auto length = *(int *)_recvBuff;
    if (length > MSG_BUFF_SIZE) {
        std::cout << ErrCode2Str(EDB_RECV_DATA_LENGTH_ERROR) << std::endl;
        return EDB_RECV_DATA_LENGTH_ERROR;
    }

    // second receive the follow-up data
    ret = RcvProc(socket, &_recvBuff[MSG_LENGTH_OCCUPY], length - MSG_LENGTH_OCCUPY);
    return ret;
}

int ICmd::RcvProc(std::unique_ptr<Socket>& socket, char *buff, int buffSize)
{
    int ret;
    while (true) {
        ret = socket->Receive(buff, buffSize);
        if (ret == EDB_TIMEOUT) {
            continue;
        } else if (ret == EDB_NETWORK_CLOSE) {
            std::cout << ErrCode2Str(EDB_SOCK_REMOTE_CLOSED) << std::endl;
            return EDB_SOCK_REMOTE_CLOSED;
        } else {
            break;
        }
    }

    return ret;
}

int ICmd::SendOrder(std::unique_ptr<Socket>& socket, MsgBuildFunc &msgBuildFunc)
{
    bson::BSONObj bsonData;
    try {
        bsonData = bson::fromjson(_json);
    } catch (std::exception &e) {
        std::cout << ErrCode2Str(EDB_INVALID_RECORD) << std::endl;
        return EDB_INVALID_RECORD;
    }

    // first build msg
    memset(_sendBuff, 0, MSG_BUFF_SIZE);
    int size = MSG_BUFF_SIZE;
    char *sendBuf = _sendBuff;
    auto ret = msgBuildFunc(sendBuf, size, bsonData);
    if (ret != EDB_OK) {
        std::cout << ErrCode2Str(EDB_MSG_BUILD_FAILED) << std::endl;
        return EDB_MSG_BUILD_FAILED;
    }

    // second send the follow-up data
    ret = socket->Send(sendBuf, *(int *)sendBuf);
    if (ret != EDB_OK) {
        std::cout << ErrCode2Str(EDB_SOCK_SEND_FAILD) << std::endl;
        return EDB_SOCK_SEND_FAILD;
    }
    return ret;
}

int ICmd::SendOrder(std::unique_ptr<Socket>& socket, int opCode)
{
    int ret = EDB_OK;
    memset(_sendBuff, 0, MSG_BUFF_SIZE);
    char *sendBuf = _sendBuff;
    auto str = "hello world";
    *(int *)sendBuf = strlen(str) + 1 + MSG_LENGTH_OCCUPY;
    strcpy(&sendBuf[MSG_LENGTH_OCCUPY], str);
    /* MsgHeader *header = (MsgHeader*)pSendBuf;
    header->messageLen = sizeof(MsgHeader);
    header->opCode = opCode; */
    ret = socket->Send(sendBuf, *(int *)sendBuf);
    return ret;
}