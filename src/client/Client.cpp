#include "Client.h"
#include <algorithm>
#include <iostream>
#include "cmd/CmdInf.h"
#include "ErrorCode.h"
#include "pd/Log.h"
#include "utils/ClassRegister.h"

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
        switch (ele) {
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
        EMDB_LOG(E) << ErrCode2Str(EDB_INVALID_PARAM);
        return EDB_INVALID_PARAM;
    }

    auto cmd = GET_REGISTER_CLASS(ICmd, input[0]);
    if (cmd == nullptr) {
        EMDB_LOG(E) << ErrCode2Str(EDB_INVALID_PARAM) << ", cmd is: " << input[0];
        return EDB_INVALID_PARAM;
    }

    auto cmdArgs = std::vector<std::string>(input.begin() + 1, input.end());
    auto ret = cmd->Execute(_socket, cmdArgs);
    if (ret != EDB_OK) {
        EMDB_LOG(E) << "failed to execute cmd: " << input[0];
    }

    EMDB_LOG(I) << "start to execute cmd: " << input[0];
    return ret;
}
