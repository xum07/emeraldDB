#include <iostream>
#include "Client.h"
#include "pd/Log.h"

using namespace EMDB;

int main(int argc, char** argv)
{
    auto ret = Log::Init(argv[0]);
    if (!ret) {
        std::cout << "failed to init log module" << std::endl;
        return -1;
    }

    Singleton<Client>::Instance().Start();
    Log::Destory();

    return 0;
}