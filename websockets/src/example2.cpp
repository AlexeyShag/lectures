#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>
#include <vector>
#include <string>

const char* http_handshake = 
"GET / HTTP/1.1\r\n"
"Host: localhost:10000\r\n"
"Upgrade: websocket\r\n"
"Connection: upgrade\r\n"
"Sec-WebSocket-Key: DY7Kyb4K7gfeXKW0Twrz+Q==\r\n"
"Sec-WebSocket-Version: 13\r\n\r\n";

struct ws_header_t {
    uint8_t opcode : 4;
    uint8_t rsv3 : 1;
    uint8_t rsv2 : 1;
    uint8_t rsv1 : 1;
    uint8_t fin : 1;
    uint8_t len : 7;
    uint8_t masked : 1;
    uint32_t mask;
};

const char message[] = "Hello world!";

int main() {
    int sockfd = -1;

    int port = 10000;
    const char* ip = "127.0.0.1";

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("couldn't create socket");
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (::connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("couldn't connect");
    }

    std::cerr << http_handshake << '\n';
    send(sockfd, http_handshake, strlen(http_handshake), 0);

    char buffer[1024];
    int recv_size = read(sockfd, buffer, sizeof(buffer));
    if (recv_size < 0) {
        throw std::runtime_error("error in recv");
    }
    buffer[recv_size] = 0;
    std::cout << buffer << '\n';

    memset(buffer, 0, 1024);
    
    ws_header_t* header = reinterpret_cast<ws_header_t*>(buffer);
    header->fin = 1;
    header->rsv1 = 0;
    header->rsv2 = 0;
    header->rsv3 = 0;
    header->opcode = 1; // text message
    header->masked = 1;
    header->len = sizeof(message);
    header->mask = 0;
    memcpy(buffer + 6, message, sizeof(message));

    for (int i = 0; i < 24; ++i) {
        std::cerr <<((buffer[i / 8] & (1 << (i % 8))) != 0);
    }
    std::cerr << '\n';

    send(sockfd, buffer, sizeof(message) + sizeof(ws_header_t), 0);

    recv_size = read(sockfd, buffer, sizeof(buffer));
    if (recv_size < 0) {
        throw std::runtime_error("error in recv");
    }
    buffer[recv_size] = 0;
    std::cout << buffer << '\n';

    return 0;
}