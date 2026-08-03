#include "command_processor.h"

std::string CommandProcessor::process(const ParsedCommand& cmd)
{
    if (cmd.command == "PING")
    {
        return "+PONG\r\n";
    }

    if(cmd.command == "SET")
    {
        if(cmd.arguments.size() != 2)
        {
            return "-ERR wrong number of arguments\r\n";
        }

        database.set(cmd.arguments[0], cmd.arguments[1]);

        return "+OK\r\n";
    }

    if(cmd.command == "GET")
    {
        if(cmd.arguments.size() != 1)
        {
            return "-ERR wrong number of arguments\r\n";
        }

        std::string value;

        if(!database.get(cmd.arguments[0], value))
        {
            return "(nil)\r\n";
        }

        return value + "(nil)\r\n";
    }

    return "-ERR unknown command\r\n";
}