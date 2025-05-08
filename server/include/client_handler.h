//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <asio/ip/tcp.hpp>
#include <iostream>

namespace cppchat::server {

using asio::ip::tcp;

class ClientHandler {
public:
    ClientHandler(tcp::socket socket);
    void start();

private:
    tcp::socket socket_;
};

} // cppchat

#endif //CLIENT_HANDLER_H
