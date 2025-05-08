//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/client_handler.h"

namespace cppchat::server {
    ClientHandler::ClientHandler(tcp::socket socket): socket_(std::move(socket)) {
    }

    void ClientHandler::start() {
        char data[1024];
        std::error_code ec;

        for (;;) {
            size_t length = socket_.read_some(asio::buffer(data), ec);

            if (ec) {
                if (ec == asio::error::eof) {
                    std::cout << "Client disconnected gracefully." << std::endl;
                } else if (ec == asio::error::connection_reset) {
                    std::cout << "Client disconnected abruptly." << std::endl;
                } else {
                    std::cerr << "Read error: " << ec.message() << std::endl;
                }
                break;
            }

            if (length > 0) {
                std::cout << "Received message: " << std::string(data, length) << std::endl;
            } else {
                std::cout << "Client disconnected (0 read)." << std::endl;
                break;
            }
        }
    }
} // cppchat::server