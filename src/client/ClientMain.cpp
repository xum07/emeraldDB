#include "Client.h"

using namespace EMDB;

int main()
{
    Singleton<Client>::Instance().Start();
    return 0;
}