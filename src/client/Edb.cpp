#include "Edb.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "cmd/CmdInf.h"
#include "utils/ClassRegister.h"
#include "ErrorCode.h"

using namespace EMDB;

constexpr int CMD_BUFFER_SIZE = 512;

Edb::Edb()
{
    _cmdBuff = std::make_unique<char []>(CMD_BUFFER_SIZE);
}

void Edb::Start()
{
    std::cout << "Welcome to EmeraldDB Shell!" << std::endl;
    std::cout << "You can type help to get help, Ctrl+C or quit to exit" << std::endl;
    while (!_quit) {
        std::cout << "emdb>";
        auto input = ReadInput();
        auto ret = CmdDispatch(input);
        if (ret != EDB_OK) {
            std::cout << "failed to execute cmd, ret=" << ret << std::endl;
        }
    }
}

std::vector<std::string> Edb::ReadInput()
{
    std::string input;
    std::getline(std::cin, input);

    // replace tab with space and delete '\'
    stringstream ss;
    for (auto& ele : input) {
        switch(ele) {
            case '\\':
                break;
            case '\t':
                ss << " ";
                break;
            default:
                ss << ele;
                break;
        }
    }

    // split by space
    std::vector<std::string> result;
    std::string word;
    while (std::getline(ss, word, ' ')) {
        result.emplace_back(word);
    }

    return result;
}

int Edb::CmdDispatch(std::vector<std::string>& input)
{
    if (input.size() < 1) {
        std::cout << ErrCode2Str(EDB_INVALID_PARAM) << std::endl;
        return EDB_INVALID_PARAM;
    }

    auto cmd = GET_REGISTER_CLASS(ICmd, input[0]);
    if (cmd == nullptr) {
        std::cout << ErrCode2Str(EDB_INVALID_PARAM) << ", cmd is: " << input[0] << std::endl;
        return EDB_INVALID_PARAM;
    }

    auto cmdArgs = std::vector<std::string>(input.begin() + 1, input.end());
    std::cout << "start to execute cmd: " << input[0] << std::endl;
    return cmd->Execute(_socket, cmdArgs);
}
