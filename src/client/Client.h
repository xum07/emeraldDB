#ifndef EMERALDDB_CLIENT_H
#define EMERALDDB_CLIENT_H

#include <atomic>
#include <iostream>
#include <istream>
#include <memory>
#include <string>
#include <vector>
#include "oss/Socket.h"
#include "utils/Singleton.h"

namespace EMDB {
class Client : public Singleton<Client> {
public:
    Client() = default;
    ~Client() override = default;
    void Start();
    void Quit() { _quit = true; };
    int InputProc(std::istream& stream = std::cin);

private:
    std::vector<std::string> ReadInput(std::istream& stream = std::cin);
    int CmdDispatch(std::vector<std::string>& input);

private:
    std::unique_ptr<Socket> _socket{std::make_unique<Socket>()};
    bool _quit{false};
};
}  // namespace EMDB

#endif  //EMERALDDB_CLIENT_H
