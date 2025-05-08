//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <iostream>

namespace cppchat::server {

using asio::ip::tcp;

class ClientHandler {
public:
    explicit ClientHandler(tcp::socket socket);
    void start();

private:
    tcp::socket socket_;
    bool handle_socket_error(const std::error_code& ec);
};

} // cppchat

#endif //CLIENT_HANDLER_H
