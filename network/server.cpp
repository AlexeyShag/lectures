#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <set>
#include <random>
#include <optional>

#include "helpers/socket.hpp"

class server_t {
public:
    server_t() {
        if ((sockfd_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("couldn't create socket");
        }
    }

    void bind(int _port) {

        int optval = 1;
        if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0) {
            throw std::runtime_error("error on set option");
        }

        sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(_port);
        if (::bind(sockfd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  {
            throw std::runtime_error("error on binding");
        }

        if (fcntl(sockfd_, F_SETFL, O_NONBLOCK)) {
            throw std::runtime_error("error on nonblock");
        }
    }

    std::optional<socket_t> try_get_client() {
        if (listen(sockfd_, 10000) < 0) {
            return std::nullopt;
        }

        sockaddr_in cli_addr;
        socklen_t clilen;

        int newsockfd = accept(sockfd_, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            return std::nullopt;
        }

        return socket_t(newsockfd);
    }

    ~server_t() {
        close(sockfd_);
    }

private:
    int sockfd_{0};
};

std::mt19937 rnd(time(0));

int main(int argc, char *argv[])
{
    server_t server;

    server.bind(2000);

    using set_t = std::set<std::pair<int, socket_t>>;
    set_t client_pool;

    std::cout << "server ready!\n";

    while (true) {
        auto client = server.try_get_client();
        if (client) {
            int priority = rnd() % 1'000'000 + 1'000'0;
            client_pool.insert({priority, std::move(*client)});
        }

        if (!client_pool.empty()) {
            auto connect = client_pool.extract(client_pool.begin());
            auto priority = connect.value().first;
            socket_t current_client(std::move(connect.value().second));
            --priority;
            if (priority == 0) {
                auto message = current_client.recv();
                std::cout << "recv message: " << message << '\n';
                current_client.send(message);
            } else {
                client_pool.insert({priority, std::move(current_client)});
            }
        }
    }
}
