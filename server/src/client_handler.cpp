//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/client_handler.h"
#include <server.h>

namespace cppchat::server {
    ClientHandler::ClientHandler(tcp::socket socket, Server *server): socket_(std::move(socket)), server_(server) {
    }

    void ClientHandler::start() {
        std::error_code ec;

        authorize(ec);

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

            std::cout << message << std::endl;

            auto j = nlohmann::json::parse(message);
            auto msg = j.get<api::Message>();

            if (msg.receiver.has_value() && msg.receiver.value() == "user_2") {
                print_message(&msg);
                continue;
            }

            server_->route_message(msg);
        }
    }

    void ClientHandler::send_message(const api::Message &msg) {
        nlohmann::json j = msg;
        std::string data = j.dump();

        uint32_t length = htonl(static_cast<uint32_t>(data.size()));

        std::error_code ec;

        asio::mutable_buffer length_buffer = asio::buffer(&length, sizeof(uint32_t));
        asio::write(socket_, length_buffer, ec);
        if (handle_socket_error(ec)) return;

        asio::mutable_buffer data_buffer = asio::buffer(data.data(), data.size());
        asio::write(socket_, data_buffer, ec);
        if (handle_socket_error(ec)) return;

        std::cout << "Message forwarded to client!" << std::endl;
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

        std::cout << "Registering client with username: " << username << std::endl;
        server_->register_client(username, shared_from_this());
    }

    void ClientHandler::print_message(api::Message *msg) {
        std::cout << "Sender: " << msg->sender << std::endl;
        if (msg->receiver.has_value())
            std::cout << "Receiver: " << msg->receiver.value() << std::endl;
        if (msg->group_id.has_value())
            std::cout << "Group ID: " << msg->group_id.value() << std::endl;
        std::cout << "Content: " << msg->content << std::endl;
        std::cout << "Timestamp: " << msg->timestamp << std::endl;
    }
} // cppchat::server
