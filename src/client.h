#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
public:
    explicit Client(int socket);

    int getSocket() const;

    std::string& inputBuffer();

    std::string& outputBuffer();

private:
    int socket;

    std::string input;

    std::string output;
};

#endif