//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include "json.hpp"
#include <iostream>
#include <message.h>

namespace cppchat::server {
    using asio::ip::tcp;

    ///
    /// @brief handles incoming client data
    class ClientHandler {
    public:
        explicit ClientHandler(tcp::socket socket);

        void start();

    private:
        tcp::socket socket_;

        static bool handle_socket_error(const std::error_code &ec);

        void print_message(api::Message *msg);
    };
} // cppchat

#endif //CLIENT_HANDLER_H
