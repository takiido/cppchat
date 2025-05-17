//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include <message.h>
#include <server.h>
#include <utils.h>

using cppchat::server::Server;

int main() {
    const int port = cppchat::api::get_env_int("PORT");

    Server sv(port);
    sv.run();
}
