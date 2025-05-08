//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef SERVER_H
#define SERVER_H

#include <list>
#include "client_handler.h"

namespace cppchat::server {

class Server {
public:
    explicit Server(int port);
    void run();

private:
    int port_;
    std::list<std::shared_ptr<ClientHandler>> clients_;
};
} // cppchat::server

#endif //SERVER_H
