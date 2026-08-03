#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <string>
#include "parser.h"

class CommandProcessor
{
public:
    std::string process(const ParsedCommand& command);
};

#endif