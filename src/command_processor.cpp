#include "command_processor.h"

std::string CommandProcessor::process(const std::string& command)
{
    if (command == "PING\r\n" || command == "PING\n")
    {
        return "+PONG\r\n";
    }

    return "-ERR unknown command\r\n";
}