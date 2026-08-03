#include "server.h"

#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server()
{
    serverSocket = -1;
}

Server::~Server()
{
    if(serverSocket != -1)
    {
        close(serverSocket);
    }
}

bool Server::createSocket()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket == -1){
        std::cerr << "Socket creation failed.\n";
        return false;
    }

    std::cout<< "Socket created.\n";
    return true;
}

bool Server::bindSocket()
{
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6379);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket,
    reinterpret_cast<sockaddr*>(&serverAddress),sizeof(serverAddress))==-1)
    {
        std::cerr<<"Bind failed.\n";
        return false;
    }

    std::cout<<"Bound to port 6379.\n";

    return true;
}


bool Server::listenForConnections(){
    if(listen(serverSocket, 5) == -1){
        std::cerr << "Listen failed.\n";
        return false;
    }

    std::cout<<"Listening...\n";

    return true;
}


void Server::handleClient(int clientSocket){
    while(true){
        char buffer[1024]{};

        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

        if(bytesReceived == 0){
            std::cout<<"Client disconnected.\n";
            break;
        }

        if(bytesReceived < 0){
            std::cerr << "Receive failed.\n";
            break;
        }

        std::cout << "Received: "<<buffer<<"\n";

        std::string command(buffer);

        std::string response = processor.process(command);

        if(send(clientSocket,response.c_str(), response.length(), 0)==-1){
            std::cerr<< "Send failed.\n";
            break;
        }
    }
    close(clientSocket);
}


void Server::acceptClients(){
    while(true){
        std::cout << "\nwaiting for client...\n";

        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);

        int clientSocket = 
            accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress),
        &clientLength);

        if(clientSocket == -1){
            std::cerr << "Accept failed\n";
            continue;
        }

        std::cout << "Client connected.\n";
        handleClient(clientSocket);
    }
}

void Server::start(){
    if(!createSocket())
        return;

    if(!bindSocket())
        return;

    if(!listenForConnections())
        return;

    acceptClients();
}









