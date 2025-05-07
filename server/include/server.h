//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef SERVER_H
#define SERVER_H

namespace cppchat::server {

class Server {
public:
    explicit Server(int port);
    void run() const;

private:
    int port;
};
} // cppchat::server

#endif //SERVER_H
