#include <iostream>

#include "server.h"

int main(){
    std::cout<< "===============================\n";
    std::cout<< "     Myredis server\n";
    std::cout<< "=============================\n";

    Server server;

    server.start();

    return 0;
}