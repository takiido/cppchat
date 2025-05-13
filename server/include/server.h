//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <logger.h>
#include "client_handler.h"

namespace cppchat::server {
    class ClientHandler;
}

namespace cppchat::server {
    ///
    /// @brief literally main server class
    class Server {
    public:
        explicit Server(int port);

        void run();

        void route_message(api::Message &msg);

        void register_client(std::string &username, std::shared_ptr<ClientHandler> client);

    private:
        int port_;
        std::list<std::shared_ptr<ClientHandler> > clients_;
        std::unordered_map<std::string, std::shared_ptr<ClientHandler> > clients_by_username;
        std::shared_ptr<spdlog::logger> logger_;
    };
} // cppchat::server

#endif //SERVER_H
