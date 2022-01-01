#include "Client.h"
#include "cmd/CmdInf.h"
#include "utils/ClassRegister.h"

using namespace EMDB;

int main()
{
    auto cmd = GET_REGISTER_CLASS(ICmd, COMMAND_QUIT);
    std::cout << (cmd == nullptr) << std::endl;
    Singleton<Client>::Instance().Start();
    return 0;
}