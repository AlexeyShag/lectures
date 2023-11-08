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
#include "helpers/epoll.hpp"
#include "helpers/socket.hpp"

int main() {
    
    size_t num_of_connections = 5;
    epoll_t epoll;

    printf("write message : ");
    std::string message;
    std::cin >> message;

    for (size_t i = 0; i < num_of_connections; ++i) {
        socket_t socket;
        socket.connect("127.0.0.1", 2000);
        socket.set_tag(i);
        socket.send(message);
        epoll.add_socket(std::move(socket));
    }

    while (num_of_connections != 0) {
        auto socket = epoll.get_ready_socket();
        if (socket) {
            message = socket->recv();
            std::cout << socket->get_tag() << " socket recv message: " << message << '\n';
            --num_of_connections;
        }
    }
	return 0;
}