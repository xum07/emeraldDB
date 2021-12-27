#include "CmdInf.h"
#include "ErrorCode.h"

using namespace EMDB;

constexpr int BUF_SIZE = 4096;

ICmd::ICmd()
{
    _recvBuf = std::make_unique<char[]>(BUF_SIZE);
    _sendBuf = std::make_unique<char[]>(BUF_SIZE);
}

int ICmd::RcvReply(Socket &sock)
{
    memset(_recvBuf, 0, RECV_BUF_SIZE);
    if (!sock.IsConnected()) {
        std::cout << ErrCode2Str(EDB_SOCK_NOT_CONNECT) << std::endl;
        return EDB_SOCK_NOT_CONNECT;
    }

    // first receive the length of the data.
    ret = RcvProc(sock, _recvBuf, sizeof(int));
    auto length = *(int *)_recvBuf;
    if (length > BUF_SIZE) {
        std::cout << ErrCode2Str(EDB_RECV_DATA_LENGTH_ERROR) << std::endl;
        return EDB_RECV_DATA_LENGTH_ERROR;
    }

    // second receive the last data.
    ret = RcvProc(sock, &_recvBuf[sizeof(int)], length - sizeof(int));
    return ret;
}

int ICmd::RcvProc(Socket &sock, char *buff, int buffSize)
{
    int ret;
    while (true) {
        ret = sock.Receive(buff, buffSize);
        if (ret == EDB_TIMEOUT) {
            continue;
        } else if (ret = EDB_NETWORK_CLOSE) {
            std::cout << ErrCode2Str(EDB_SOCK_REMOTE_CLOSED) << std::endl;
            return EDB_SOCK_REMOTE_CLOSED;
        } else {
            break;
        }
    }

    return ret;
}

int ICmd::SendOrder(Socket &sock, MsgBuildFunc &msgBuildFunc)
{
    bson::BSONObj bsonData;
    try {
        bsonData = bson::fromjson(_jsonString);
    } catch (std::exception &e) {
        std::cout << ErrCode2Str(EDB_INVALID_RECORD) << std::endl;
        return EDB_INVALID_RECORD;
    }

    memset(_sendBuf, 0, BUF_SIZE);
    int size = BUF_SIZE;
    char *pSendBuf = _sendBuf;
    auto ret = msgBuildFunc(&pSendBuf, &size, bsonData);
    if (ret != EDB_OK) {
        std::cout << ErrCode2Str(EDB_MSG_BUILD_FAILED) << std::endl;
        return EDB_MSG_BUILD_FAILED;
    }

    ret = sock.Send(pSendBuf, *(int *)pSendBuf);
    if (ret != EDB_OK) {
        std::cout << ErrCode2Str(EDB_SOCK_SEND_FAILD) << std::endl;
        return EDB_SOCK_SEND_FAILD;
    }
    return ret;
}

int ICmd::SendOrder(Socket &sock, int opCode)
{
    int ret = EDB_OK;
    memset(_sendBuf, 0, BUF_SIZE);
    char *pSendBuf = _sendBuf;
    const char *pStr = "hello world";
    *(int *)pSendBuf = strlen(pStr) + 1 + sizeof(int);
    memcpy(&pSendBuf[4], pStr, strlen(pStr) + 1);
    /* MsgHeader *header = (MsgHeader*)pSendBuf;
    header->messageLen = sizeof(MsgHeader);
    header->opCode = opCode; */
    ret = sock.Send(pSendBuf, *(int *)pSendBuf);
    return ret;
}