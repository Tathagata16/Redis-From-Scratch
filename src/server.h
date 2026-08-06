#ifndef SERVER_H
#define SERVER_H

#include "command_processor.h"
#include "database.h"
#include "client.h"
#include <unordered_map>

class Server
{
public:
    Server();
    ~Server();

    void start();

private:
    int serverSocket;

    Database database;

    int epollFd;

    std::unordered_map<int, Client> clients;

    CommandProcessor processor;

    bool createSocket();
    bool bindSocket();
    bool listenForConnections();
    bool setNonBlocking(int socket);
    bool createEpoll();
    bool acceptNewClient();

    void acceptClients();
    void handleClient(Client& client);
};

#endif