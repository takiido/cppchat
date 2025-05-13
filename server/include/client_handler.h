//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/write.hpp>
#include "json.hpp"
#include <iostream>
#include <message.h>
#include <server.h>

namespace cppchat::server {
    class Server;
}

namespace cppchat::server {
    using asio::ip::tcp;

    ///
    /// @brief handles incoming client data
    class ClientHandler: public std::enable_shared_from_this<ClientHandler>{
    public:
        explicit ClientHandler(tcp::socket socket, server::Server *server);

        void start();

        void send_message(api::Message &msg);

    private:
        tcp::socket socket_;
        server::Server *server_;
        std::string username_;
        static std::shared_ptr<spdlog::logger> logger_;

        static bool handle_socket_error(const std::error_code &ec);

        void print_message(api::Message &msg);

        void authorize(std::error_code &ec);

        bool get_message(std::error_code &ec);
    };
} // namespace cppchat::server

#endif //CLIENT_HANDLER_H
