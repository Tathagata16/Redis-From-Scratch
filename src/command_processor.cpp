#include "command_processor.h"

CommandProcessor::CommandProcessor(Database* db):database(db){}

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

        database->set(cmd.arguments[0], cmd.arguments[1]);

        return "+OK\r\n";
    }

    if(cmd.command == "GET")
    {
        if(cmd.arguments.size() != 1)
        {
            return "-ERR wrong number of arguments\r\n";
        }

        std::string value;

        if(!database->get(cmd.arguments[0], value))
        {
            return "(nil)\r\n";
        }

        return value + "(nil)\r\n";
    }

    //del command
    if(cmd.command == "DEL")
    {
        if(cmd.arguments.size() != 1){
            return "-ERR wrong number of arguments\r\n";
        }

        if(database->remove(cmd.arguments[0])){
            return ":1\r\n";
        }

        return ":0\r\n";
    }

    if(cmd.command == "EXISTS")
    {
        if(cmd.arguments.size() != 1)
        {
            return "-ERR wrong number of arguments\r\n";
        }

        if(database->exists(cmd.arguments[0]))
        {
            return ":1\r\n";
        }

        return ":0\r\n";
    }

    if (cmd.command == "EXPIRE")
    {
        if (cmd.arguments.size() != 2)
        {
            return "-ERR wrong number of arguments\r\n";
        }

        int seconds = std::stoi(cmd.arguments[1]);

        if (database->expire(cmd.arguments[0], seconds))
        {
            return ":1\r\n";
        }

        return ":0\r\n";
    }

    return "-ERR unknown command\r\n";
}