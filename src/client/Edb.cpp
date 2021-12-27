#include "Edb.h"
#include <iostream>
#include <sstream>
#include <algorithm>

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
    }
}

void Edb::Prompt()
{
    std::cout << "emdb>";
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
    while (ss >> word) {
        result.emplace_back(word);
    }

    return result;
}
