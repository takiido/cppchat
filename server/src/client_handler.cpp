//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/client_handler.h"

namespace cppchat::server {
    ClientHandler::ClientHandler(tcp::socket socket): socket_(std::move(socket)) {
    }

    void ClientHandler::start() {
        std::error_code ec;

        while (true) {
            uint32_t length = 0;
            asio::read(socket_, asio::buffer(&length, sizeof(length)), ec);

            if (handle_socket_error(ec)) break;

            length = ntohl(length);

            if (length == 0 || length > 1024) {
                std::cout << "Invalid message length received: " << length << std::endl;
                break;
            }

            std::vector<char> data(length);
            asio::read(socket_, asio::buffer(data), ec);
            if (handle_socket_error(ec)) break;

            std::string message(data.begin(), data.end());
            std::cout << "Message received: " << message << std::endl;
        }
    }

    bool ClientHandler::handle_socket_error(const std::error_code &ec) {
        if (!ec) return false;

        if (ec == asio::error::eof) {
            std::cout << "Client disconnected gracefully." << std::endl;
        } else if (ec == asio::error::connection_reset) {
            std::cout << "Client disconnected abruptly." << std::endl;
        } else {
            std::cerr << "Read error: " << ec.message() << std::endl;
        }
        return true;
    }
} // cppchat::server