#ifndef EMERALDDB_QUIT_COMMAND_H
#define EMERALDDB_QUIT_COMMAND_H

#include "CmdInf.h"

namespace EMDB {

class QuitCmd : public ICmd {
public:
    int Execute(Socket &sock, std::vector<std::string> &args) override;

protected:
    int HandleReply() override;
};

}  // namespace EMDB

#endif EMERALDDB_QUIT_COMMAND_H