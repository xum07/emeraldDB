#include "Client.h"
#include <iostream>
#include <algorithm>
#include "cmd/CmdInf.h"
#include "utils/ClassRegister.h"
#include "ErrorCode.h"

using namespace EMDB;

void Client::Start()
{
    std::cout << "Welcome to EmeraldDB Shell!" << std::endl;
    std::cout << "You can type help to get help, Ctrl+C or quit to exit" << std::endl << std::endl;
    while (!_quit) {
        (void)InputProc();
    }
}

int Client::InputProc(std::istream& stream)
{
    std::cout << "emdb>";
    auto input = ReadInput(stream);
    return CmdDispatch(input);
}

std::vector<std::string> Client::ReadInput(std::istream& stream)
{
    std::string input;
    std::getline(stream, input);

    // replace tab with space and delete '\\'(used as a line break mark)
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

int Client::CmdDispatch(std::vector<std::string>& input)
{
    if (input.empty()) {
        std::cout << ErrCode2Str(EDB_INVALID_PARAM) << std::endl;
        return EDB_INVALID_PARAM;
    }

    auto cmd = GET_REGISTER_CLASS(ICmd, input[0]);
    if (cmd == nullptr) {
        std::cout << ErrCode2Str(EDB_INVALID_PARAM) << ", cmd is: " << input[0] << std::endl;
        return EDB_INVALID_PARAM;
    }

    auto cmdArgs = std::vector<std::string>(input.begin() + 1, input.end());
    auto ret = cmd->Execute(_socket, cmdArgs);
    if (ret != EDB_OK) {
        std::cout << "failed to execute cmd: " << input[0] << std::endl;
    }

    return ret;
}
