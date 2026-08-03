#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <string>

class CommandProcessor
{
public:
    std::string process(const std::string& command);
};

#endif