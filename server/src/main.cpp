//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include <server.h>

using cppchat::server::Server;

int main() {
    Server sv(12345);
    sv.run();
}
