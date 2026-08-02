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

    std::cout<< "Listening for connections...\n";

    std::cout<< "Press enter to exit...\n";
    std::cin.get();


    //close before exit
    close(serverSocket);

    return 0;
}