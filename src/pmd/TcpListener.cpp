#include "TcpListener.h"
#include <memory>
#include "ErrorCode.h"
#include "oss/Socket.h"
#include "pd/Log.h"

namespace EMDB {
int SimpleSrvRun()
{
    constexpr uint32_t SERVER_PORT = 51201;
    Socket socket(SERVER_PORT);
    auto ret = socket.InitSocket();
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "failed to init sock, ret=" << ret;
        return ret;
    }

    ret = socket.BindAndListen();
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "failed to bind/listen sock, ret=" << ret;
        return ret;
    }

    while (true) {
        int sock;
        ret = socket.Accept(sock, nullptr, nullptr);
        if (ret == EDB_TIMEOUT) {
            ret = EDB_OK;
            continue;
        }

        Socket socketTmp(sock);
        socketTmp.DisableNagle();
        // first receive length
        int size;
        do {
            ret = socketTmp.Receive((char*)&size, sizeof(int));
            if ((ret != EDB_OK) && (ret != EDB_TIMEOUT)) {
                EMDB_LOG(E) << "failed to receive size, ret=" << ret;
                return ret;
            }
        } while (ret == EDB_TIMEOUT);

        // second receive follow message
        auto buff = std::make_unique<char[]>(1024);
        do {
            ret = socketTmp.Receive(buff.get(), size - sizeof(int));
            if ((ret != EDB_OK) && (ret != EDB_TIMEOUT)) {
                EMDB_LOG(E) << "failed to receive buff, ret=" << ret;
                return ret;
            }
        } while (ret == EDB_TIMEOUT);

        EMDB_LOG(I) << "received buff is: " << buff.get();
        socketTmp.Close();
    }

    return EDB_OK;
}
}  // namespace EMDB