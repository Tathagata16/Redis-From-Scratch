#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <string>
#include "parser.h"
#include "database.h"

class CommandProcessor
{
public:
    explicit CommandProcessor(Database* db);
    
    std::string process(const ParsedCommand& command);
private:
    Database* database;
};

#endif