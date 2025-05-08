//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include <message.h>
#include <server.h>
#include <utils.h>

using cppchat::server::Server;

int main() {
    auto env = cppchat::api::load_env("../../.env.dev");

    Server sv(std::stoi(env["PORT"]));
    sv.run();
}
