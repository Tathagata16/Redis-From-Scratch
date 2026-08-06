#include "server.h"

#include <iostream>
#include <cstring>

#include "parser.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(): serverSocket(-1), processor(&database){}


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


void Server::handleClient(Client& client){
    while(true){
        char tempBuffer[1024]{};

        ssize_t bytesReceived = recv(client.getSocket(), tempBuffer, sizeof(tempBuffer)-1, 0);

        if(bytesReceived == 0){
            std::cout<<"Client disconnected.\n";
            break;
        }

        if(bytesReceived < 0){
            std::cerr << "Receive failed.\n";
            break;
        }

        client.inputBuffer().append(tempBuffer, bytesReceived);

        std::cout << "Received: "
                << client.inputBuffer()
                << "\n";

        Parser parser;

        std::string input = client.inputBuffer();

        ParsedCommand command = parser.parse(input);
        std::cout<< "\nCommand: "<<command.command <<"\n";

        std::cout<<"Arguments:\n";

        for(const auto& arg:command.arguments){
            std::cout<< " " << arg << "\n";
        }

        std::string response = processor.process(command);

        if(send(client.getSocket(),response.c_str(), response.length(), 0)==-1){
            std::cerr<< "Send failed.\n";
            break;
        }

        client.inputBuffer().clear();
    }
    close(client.getSocket());
}


void Server::acceptClients(){
    while(true){
        std::cout << "\nwaiting for client...\n";

        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);

        int clientSocket = accept(
            serverSocket,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &clientLength);

        if (clientSocket == -1)
        {
            std::cerr << "Accept failed.\n";
            continue;
        }

        Client client(clientSocket);

        handleClient(client);
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









