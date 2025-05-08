//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/server.h"
#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

namespace cppchat::server {
    Server::Server(const int port) {
        this->port_ = port;
    }

    void Server::run() {
        try {
            asio::io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port_));
            std::cout << "Server started on port " << port_ << std::endl;

            for (;;) {
                tcp::socket socket(io_context);
                acceptor.accept(socket); // Waiting for next new client

                std::cout << "New connection from " << socket.remote_endpoint().address() << ":" << socket.
                        remote_endpoint().port() << std::endl;

                // Create handler for next new client
                //TODO: test handler from telnet
                auto handler = std::make_shared<ClientHandler>(std::move(socket));
                clients_.push_back(handler);
                handler->start(); // Ask client handler to do his handlin staff
            }
        } catch (const std::exception &e) {
            std::cerr << "Server error: " << e.what() << std::endl;
        }
    }
} // cppchat::server
