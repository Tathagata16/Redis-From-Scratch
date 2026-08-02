#include <iostream>

//linux networking headers
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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

    //close before exit
    close(serverSocket);

    return 0;
}