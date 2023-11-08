#pragma once 

#include "socket.hpp"
#include <sys/epoll.h>
#include <list>
#include <optional>


class epoll_t {
public:
    epoll_t() {
        epoll_fd_ = epoll_create(1);
        if (epoll_fd_ == -1) {
            throw std::runtime_error("can't create epoll fd");
        }
    }

    void add_socket(socket_t&& _socket) {
        sockets.push_back(std::move(_socket));

        epoll_event ev;

        ev.data.ptr = reinterpret_cast<void*>(&sockets.back());
        ev.events = EPOLLIN | EPOLLONESHOT;

        int error = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sockets.back().get_handle(), &ev );
        if (error < 0) {
            throw std::runtime_error("can't add socket to epoll");
        }
    }

    socket_t* get_ready_socket(size_t _timeout = 1) {
        epoll_event ev;

        int ready = epoll_wait( epoll_fd_, &ev, 1, _timeout * 1000 );

        if (ready < 0) {
            throw std::runtime_error("error in epoll_wait");
        } else if (ready == 1 && ev.events & EPOLLIN) {
            return reinterpret_cast<socket_t*>(ev.data.ptr);
        }
        return nullptr;
    }

private:
    int epoll_fd_{0};
    std::list<socket_t> sockets;
};