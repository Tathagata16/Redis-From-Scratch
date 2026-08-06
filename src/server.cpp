#include "server.h"

#include <iostream>
#include <cstring>

#include "parser.h"
#include "client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <fcntl.h>
#include <cerrno>
#include <sys/epoll.h>

Server::Server(): serverSocket(-1),epollFd(-1), processor(&database){}


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

    if (!setNonBlocking(serverSocket))
    {
        close(serverSocket);
        return false;
    }

    std::cout<< "Socket created.\n";
    return true;
}

//function to create epoll instance


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

bool Server::setNonBlocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);

    if (flags == -1)
    {
        std::cerr << "Failed to get socket flags.\n";
        return false;
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        std::cerr << "Failed to set socket as non-blocking.\n";
        return false;
    }

    return true;
}

bool Server::createEpoll()
{
    epollFd = epoll_create1(0);

    if (epollFd == -1)
    {
        std::cerr << "Failed to create epoll instance.\n";
        return false;
    }

    epoll_event event{};

    event.events = EPOLLIN;
    event.data.fd = serverSocket;

    if (epoll_ctl(
            epollFd,
            EPOLL_CTL_ADD,
            serverSocket,
            &event) == -1)
    {
        std::cerr << "Failed to register server socket.\n";
        return false;
    }

    return true;
}


void Server::handleClient(Client& client){
        char tempBuffer[1024]{};

        ssize_t bytesReceived = recv(client.getSocket(), tempBuffer, sizeof(tempBuffer)-1, 0);

        if (bytesReceived == 0)
        {
            std::cout << "Client disconnected.\n";

            epoll_ctl(
                epollFd,
                EPOLL_CTL_DEL,
                client.getSocket(),
                nullptr);

            clients.erase(client.getSocket());

            close(client.getSocket());

            return;
        }

        if (bytesReceived < 0)
        {

            std::cerr << "Receive failed.\n";

            epoll_ctl(
                epollFd,
                EPOLL_CTL_DEL,
                client.getSocket(),
                nullptr);

            clients.erase(client.getSocket());

            close(client.getSocket());

            return;
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
            std::cerr << "Send failed.\n";

            epoll_ctl(
                epollFd,
                EPOLL_CTL_DEL,
                client.getSocket(),
                nullptr);

            clients.erase(client.getSocket());

            close(client.getSocket());

            return;
        }

        client.inputBuffer().clear();
    
}

void Server::acceptNewClient()
{
    while (true)
    {
        
        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);
    
        int clientSocket =
            accept(serverSocket,
                   reinterpret_cast<sockaddr*>(&clientAddress),
                   &clientLength);

        if (clientSocket == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }

            // actual error
            std::cerr<< "Accept failed.\n";
            break;
        }

        // register client
    
        if (!setNonBlocking(clientSocket))
        {
            close(clientSocket);
            continue;
        }
    
        clients.emplace(
            clientSocket,
            Client(clientSocket));
    
        epoll_event event{};
    
        event.events = EPOLLIN;
        event.data.fd = clientSocket;
    
        epoll_ctl(
            epollFd,
            EPOLL_CTL_ADD,
            clientSocket,
            &event);
    
        std::cout << "Client connected.\n";
    }

}


void Server::acceptClients()
{
    constexpr int MAX_EVENTS = 64;

    epoll_event events[MAX_EVENTS];

    while (true)
    {
        int ready = epoll_wait(
            epollFd,
            events,
            MAX_EVENTS,
            -1);

        if (ready == -1)
        {
            std::cerr << "epoll_wait failed.\n";
            continue;
        }

        for (int i = 0; i < ready; i++)
        {
            int fd = events[i].data.fd;

            if (fd == serverSocket)
            {
                acceptNewClient();
            }
            else
            {
                auto it = clients.find(fd);

                if (it != clients.end())
                {
                    handleClient(it->second);
                }
            }
        }
    }
}

void Server::start(){
    if(!createSocket())
        return;

    if(!bindSocket())
        return;

    if(!listenForConnections())
        return;

    if(!createEpoll())return;

    acceptClients();
}









