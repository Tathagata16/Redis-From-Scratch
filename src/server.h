#ifndef SERVER_H
#define SERVER_H

#include "command_processor.h"

class Server
{
public:
    Server();
    ~Server();

    void start();

private:
    int serverSocket;

    CommandProcessor processor;

    bool createSocket();
    bool bindSocket();
    bool listenForConnections();

    void acceptClients();
    void handleClient(int clientSocket);
};

#endif