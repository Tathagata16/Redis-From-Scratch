#include "parser.h"

#include <sstream>

ParsedCommand Parser::parse(const std::string& input){
    ParsedCommand result;

    std::stringstream ss(input);

    ss>>result.command;

    std::string argument;

    while(ss >> argument){
        result.arguments.push_back(argument);
    }

    return result;
}