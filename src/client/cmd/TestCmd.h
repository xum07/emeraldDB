#ifndef EMERALDDB_TEST_COMMAND_H
#define EMERALDDB_TEST_COMMAND_H

#include "CmdInf.h"

namespace EMDB {

class TestCmd : public ICmd {
public:
    TestCmd() = default;
    ~TestCmd() override = default;
    int Execute(std::unique_ptr<Socket>& socket, std::vector<std::string>& args) override;
    std::vector<std::string> GetCmdArgs() const { return _args; }

private:
    std::vector<std::string> _args;
};

}  // namespace EMDB

#endif  // EMERALDDB_TEST_COMMAND_H