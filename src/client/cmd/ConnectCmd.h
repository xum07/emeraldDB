#ifndef EMERALDDB_CONNECT_COMMAND_H
#define EMERALDDB_CONNECT_COMMAND_H

#include "CmdInf.h"

namespace EMDB {

class ConnectCmd : public ICmd {
public:
    ConnectCmd() = default;
    ~ConnectCmd() override = default;
    int Execute(std::unique_ptr<Socket>& socket, std::vector<std::string>& args) override;

private:
    std::string _address;
    uint32_t _port;
};

}  // namespace EMDB

#endif  // EMERALDDB_CONNECT_COMMAND_H