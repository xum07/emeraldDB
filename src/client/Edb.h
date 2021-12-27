#ifndef EMERALDDB_EDB_H
#define EMERALDDB_EDB_H

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include "oss/Socket.h"

namespace EMDB {
class Edb {
public:
    Edb();
    virtual ~Edb() = default;
    void Start();
    void Quit() { _quit = true; };

private:
    std::vector<std::string> ReadInput();
    int CmdDispatch(std::vector<std::string>& input);

private:
    std::unique_ptr<Socket> _socket{nullptr};
    bool _quit{false};
};
}  // namespace EMDB

#endif //EMERALDDB_EDB_H
