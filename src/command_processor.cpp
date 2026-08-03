#include "command_processor.h"

std::string CommandProcessor::process(const ParsedCommand& cmd)
{
    if (cmd.command == "PING")
    {
        return "+PONG\r\n";
    }

    return "-ERR unknown command\r\n";
}