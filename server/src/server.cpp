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
        Logger::init();
        logger_ = spdlog::get("cppchat-server");
        logger_->info("Server started on port {}", port_);

        try {
            asio::io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port_));

            for (;;) {
                tcp::socket socket(io_context);
                acceptor.accept(socket); // Waiting for next new client

               logger_->info("New connection from {}:{}", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());

                // Create handler for next new client
                auto handler = std::make_shared<ClientHandler>(std::move(socket), this);
                clients_.push_back(handler);

                std::thread([handler]() { // Probably will work for multiple users idk
                    handler->start(); // Ask client handler to do his handlin staff
                }).detach();
            }
        } catch (const std::exception &e) {
            logger_->critical("Server error: {}", e.what());
        }
    }

    void Server::route_message(api::Message &msg) {
        if (const auto search = clients_by_username.find(msg.receiver.value_or(""));
            search != clients_by_username.end()) {
            logger_->info("Found {} {}", search->first, static_cast<void*>(search->second.get()));
            search->second->send_message(msg);
        } else {
            logger_->info("Client not found");
        }
    }

    void Server::register_client(std::string &username, std::shared_ptr<ClientHandler> client) {
        clients_by_username[username] = client;
        logger_->info("Client registered as {}", username);
    }
} // cppchat::server
