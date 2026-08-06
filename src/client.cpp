#include "client.h"

Client::Client(int socket)
    : socket(socket)
{
}

int Client::getSocket() const
{
    return socket;
}

std::string& Client::inputBuffer()
{
    return input;
}

std::string& Client::outputBuffer()
{
    return output;
}