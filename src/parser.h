#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct ParsedCommand
{
    std::string command;
    std::vector<std::string> arguments;

};

class Parser{
public:
    ParsedCommand parse(const std::string& input);
};


#endif