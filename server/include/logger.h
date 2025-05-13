//
// Created by Alex Tovshchyk on 2025-05-12.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <string>
#include <filesystem>
#include <iostream>

using std::filesystem::path, std::filesystem::create_directories;

namespace cppchat::server {
    class Logger {
    public:
        static void init(const std::string &file = "server.log");

        static std::shared_ptr<spdlog::logger>& get();

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
    };
} // namespace cppchat::server

#endif //LOGGER_H
