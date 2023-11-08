#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>
#include <vector>
#include "helpers/socket.hpp"

int main() {
    
    size_t num_of_connections = 3;

    std::vector<socket_t> sockets(num_of_connections);

    for (auto& socket: sockets) {
        socket.connect("127.0.0.1", 2000);
    }
    
	printf("connected to the server\n");

    printf("write message : ");
    std::string message;
    std::cin >> message;

    for (auto& socket: sockets) {
        socket.send(message);
    }

    for (size_t i = 0; i < sockets.size(); ++i) {
        auto& socket = sockets[i];
        message = socket.recv();
        std::cout << i << " socket recv message: " << message << '\n';
    }


	return 0;
}