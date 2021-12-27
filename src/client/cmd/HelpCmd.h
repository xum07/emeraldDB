#ifndef EMERALDDB_HELP_COMMAND_H
#define EMERALDDB_HELP_COMMAND_H

#include "CmdInf.h"

namespace EMDB {

class HelpCmd : public ICmd {
public:
    int Execute(Socket &sock, std::vector<std::string> &args) override;
};

}  // namespace EMDB

#endif EMERALDDB_HELP_COMMAND_H