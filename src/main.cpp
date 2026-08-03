#include <iostream>

//linux networking headers
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int main()
{
    std::cout << "=============================\n";
    std::cout << "       MyRedis Server\n";
    std::cout << "=============================\n";
    
    //creating a tcp socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket == -1){
        std::cerr<<"Failed to create socket.\n";
        return 1;
    }

    std::cout<<"Socket created successfully\n";
    std::cout<<"Socket Descriptor: " << serverSocket << "\n";

    //create server address
    sockaddr_in serverAddress;

    std::memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6379);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //bind sokket to port
    if(bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress))==-1){
        std::cerr<<"Failed to bind socket.\n";
        close(serverSocket);
        return 1;
    }

    std::cout<<"Socket bound to port 6379 successfully.\n";

    //step 4: Listen: 
    if(listen(serverSocket, 5) == -1){
        std::cerr<< "Listen failed.\n";
        close(serverSocket);
        return 1;
    }

    std::cout<<"Waiting for a clinent...\n";

    sockaddr_in clientAddress{};
    socklen_t clientLength = sizeof(clientAddress);

    int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress),&clientLength);

    if(clientSocket == -1){
        std::cerr << "Accept failed.\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Client connected!\n";

    //receive data

    char buffer[1024];

    std::memset(buffer, 0, sizeof(buffer));

    ssize_t bytesReceived = recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if(bytesReceived == -1){
        std::cerr<<"Receive failed.\n";
    }else{
        std::cout<<"\nReceived "<<bytesReceived<<" bytes\n";
        std::cout<<"Message : "<<buffer <<"\n";
    }


    //close before exit
    close(clientSocket);
    close(serverSocket);

    return 0;
}