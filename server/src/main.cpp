//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include <server.h>

using cppchat::server::Server;

int main() {
    const Server server(12345);
    server.run();
}
