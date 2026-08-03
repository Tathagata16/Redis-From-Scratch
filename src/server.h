#ifndef SERVER_H
#define SERVER_H

class Server
{
public:
    Server();
    ~Server();

    void start();

private:
    int serverSocket;

    bool createSocket();
    bool bindSocket();
    bool listenForConnections();

    void acceptClients();
    void handleClient(int clientSocket);
};

#endif