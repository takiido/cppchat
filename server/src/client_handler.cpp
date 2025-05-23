//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/client_handler.h"
#include <server.h>

namespace cppchat::server {
    ClientHandler::ClientHandler(tcp::socket socket, Server *server): socket_(std::move(socket)), server_(server) {
    }

    std::shared_ptr<spdlog::logger> cppchat::server::ClientHandler::logger_ = nullptr;

    void ClientHandler::start() {
        std::error_code ec;

        logger_ = spdlog::get("cppchat-server");

        authorize(ec);

        while (get_message(ec));
    }

    void ClientHandler::send_message(api::Message &msg) {
        nlohmann::json j {msg};

        std::string data = j.dump();

        uint32_t length = htonl(static_cast<uint32_t>(data.size()));

        std::error_code ec;

        asio::mutable_buffer length_buffer = asio::buffer(&length, sizeof(uint32_t));
        asio::write(socket_, length_buffer, ec);
        if (handle_socket_error(ec)) return;

        asio::mutable_buffer data_buffer = asio::buffer(data.data(), data.size());
        asio::write(socket_, data_buffer, ec);
        if (handle_socket_error(ec)) return;

        logger_->info("Message forwarded to client");
    }

    bool ClientHandler::handle_socket_error(const std::error_code &ec) {
        if (!ec) return false;

        if (ec == asio::error::eof) {
            logger_->info("Client disconnected gracefully");
        } else if (ec == asio::error::connection_reset) {
            logger_->warn("Client disconnected abruptly");
        } else {
            logger_->error("Read error: {}", ec.message());
        }
        return true;
    }

    void ClientHandler::authorize(std::error_code &ec) {
        // Trying to register user
        uint32_t length = 0;
        asio::read(socket_, asio::buffer(&length, sizeof(uint32_t)), ec);
        if (handle_socket_error(ec)) return;
        length = ntohl(length);

        std::vector<char> data(length);
        asio::read(socket_, asio::buffer(data), ec);
        if (handle_socket_error(ec)) return;

        std::string message(data.begin(), data.end());
        auto j = nlohmann::json::parse(message);
        std::string username = j["username"];

        logger_->info("Registering client with username: {}", username);
        server_->register_client(username, shared_from_this());
    }

    bool ClientHandler::get_message(std::error_code &ec) {
        uint32_t length = 0;
        asio::read(socket_, asio::buffer(&length, sizeof(uint32_t)), ec);
        if (handle_socket_error(ec)) return false;

        length = ntohl(length);

        if (length == 0 || length > 1024) {
           logger_->warn("Invalid message length received: {}", length);
            return false;
        }

        std::vector<char> data(length);
        asio::read(socket_, asio::buffer(data), ec);
        if (handle_socket_error(ec)) return false;

        std::string message(data.begin(), data.end());

        auto j = nlohmann::json::parse(message);
        api::Message msg = j.get<api::Message>();

        // TODO: add group messages logic
        if (msg.receiver.has_value()) {
            server_->route_message(msg);
        }

        return true;
    }

    void ClientHandler::print_message(api::Message &msg) {
        std::cout << "Sender: " << msg.sender << std::endl;
        if (msg.receiver.has_value())
            std::cout << "Receiver: " << msg.receiver.value() << std::endl;
        if (msg.group_id.has_value())
            std::cout << "Group ID: " << msg.group_id.value() << std::endl;
        std::cout << "Content: " << msg.content << std::endl;
        std::cout << "Timestamp: " << msg.timestamp << std::endl;
    }
} // cppchat::server
