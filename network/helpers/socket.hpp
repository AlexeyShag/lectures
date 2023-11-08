#pragma once

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

class socket_t {
public:
    socket_t() {
        if ((sockfd_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("couldn't create socket");
        }
    }

    socket_t(int _fd): sockfd_(_fd) {}

    socket_t(const socket_t&) = delete;
    socket_t& operator=(const socket_t&) = delete;

    socket_t(socket_t&& _socket){
        sockfd_ = _socket.sockfd_;
        tag_ = _socket.tag_;
        _socket.sockfd_ = 0;
    }
    socket_t& operator=(socket_t&& _socket) {
        sockfd_ = _socket.sockfd_;
        tag_ = _socket.tag_;
        _socket.sockfd_ = 0;
        return *this;
    }


    void connect(const std::string& _ip, int _port) const {
        struct sockaddr_in server_addr;

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(_port); // htons(2000);
        server_addr.sin_addr.s_addr = inet_addr(_ip.data()); //inet_addr("192.168.1.35");

        if (::connect(sockfd_, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            throw std::runtime_error("couldn't connect");
        }
    }

    void send(const std::string& _message) const {
        ::send(sockfd_, _message.data(), _message.size(), 0);
    }

    auto recv(size_t _size = 1024) const {
        std::string message;
        message.resize(_size);
        auto recv_size = ::read(sockfd_, &(message[0]), _size);
        if (recv_size < 0) {
            throw std::runtime_error("can't read message");
        }
        message.resize(recv_size);
        return message;
    }

    bool operator<(const socket_t& _socket) const {
        return sockfd_ < _socket.sockfd_;
    }

    auto get_handle() {
        return sockfd_;
    }

    void set_tag(int _tag) {
        tag_ = _tag;
    }

    auto get_tag() {
        return tag_;
    }

    ~socket_t() {
        if (sockfd_ > 0) {
            close(sockfd_);
        }
    }

private:
    int sockfd_{0};
    int tag_{0};
};