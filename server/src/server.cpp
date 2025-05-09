//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/server.h"
#include <asio.hpp>
#include <iostream>
#include <client_handler.h>

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
                auto handler = std::make_shared<ClientHandler>(std::move(socket), this);
                clients_.push_back(handler);

                std::thread([handler]() { // Probably will work for multiple users idk
                    handler->start(); // Ask client handler to do his handlin staff
                }).detach();
            }
        } catch (const std::exception &e) {
            std::cerr << "Server error: " << e.what() << std::endl;
        }
    }

    void Server::route_message(api::Message &msg) {
        if (const auto search = clients_by_username.find(msg.receiver.value_or(""));
            search != clients_by_username.end()) {
            std::cout << "Found " << search->first << ' ' << search->second << '\n';
            search->second->send_message(msg);
        } else std::cout << "Not found\n";
    }

    void Server::register_client(std::string &username, std::shared_ptr<ClientHandler> client) {
        clients_by_username[username] = client;
        std::cout << "Client registered as: " << username << std::endl;
    }
} // cppchat::server
