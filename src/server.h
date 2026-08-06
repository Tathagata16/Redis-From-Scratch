#ifndef SERVER_H
#define SERVER_H

#include "command_processor.h"
#include "database.h"
#include "client.h"

class Server
{
public:
    Server();
    ~Server();

    void start();

private:
    int serverSocket;

    Database database;

    CommandProcessor processor;

    bool createSocket();
    bool bindSocket();
    bool listenForConnections();

    void acceptClients();
    void handleClient(Client& client);
};

#endif