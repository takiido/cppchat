//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/server.h"
#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

namespace cppchat::server {
    Server::Server(const int port) {
        this->port = port;
    }

    void Server::run() const {
        try {
            asio::io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
            std::cout << "Server started on port " << port << std::endl;

            for (;;) {
                tcp::socket socket(io_context);
                acceptor.accept(socket);
                std::cout << "New connection from " << socket.remote_endpoint().address().to_string() << ":" << socket.
                        remote_endpoint().port() << std::endl;
            }

        } catch (const std::exception &e) {
            std::cerr << "Server error: " << e.what() << std::endl;
        }
    }
} // cppchat::server