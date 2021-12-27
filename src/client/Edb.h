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
    void Prompt();
    std::vector<std::string> ReadInput();

private:
    Socket _sock;
    bool _quit{false};
};
}  // namespace EMDB

#endif //EMERALDDB_EDB_H
