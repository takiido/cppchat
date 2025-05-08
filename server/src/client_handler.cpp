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

        size_t length = socket_.read_some(asio::buffer(data), ec);
        if (!ec) {
          std::cout << "Received message: " << std::string(data, length) << std::endl;
        } else {
            std::cerr << "Reading from socket failed: " << ec.message() << std::endl;
        }
    }
} // cppchat::server