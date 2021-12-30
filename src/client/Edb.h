#ifndef EMERALDDB_EDB_H
#define EMERALDDB_EDB_H

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <istream>
#include <iostream>
#include "oss/Socket.h"

namespace EMDB {
class Edb {
public:
    Edb() = default;
    virtual ~Edb() = default;
    void Start();
    void Quit() { _quit = true; };
    int InputProc(std::istream& stream = std::cin);

private:
    std::vector<std::string> ReadInput(std::istream& stream = std::cin);
    int CmdDispatch(std::vector<std::string>& input);

private:
    std::unique_ptr<Socket> _socket{nullptr};
    bool _quit{false};
};
}  // namespace EMDB

#endif //EMERALDDB_EDB_H
