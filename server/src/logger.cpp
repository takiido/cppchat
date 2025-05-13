//
// Created by Alex Tovshchyk on 2025-05-12.
//

#include "../include/logger.h"

namespace cppchat::server {
    void Logger::init(const std::string &file) {
        try {
            path log_path = path(PROJECT_ROOT) / "logs/" / file;
            create_directories(log_path.parent_path());

            if (!spdlog::get("cppchat-server")) {
                s_logger = spdlog::daily_logger_mt("cppchat-server", log_path, 0, 0);
            }

            spdlog::set_default_logger(s_logger);
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
            spdlog::set_level(spdlog::level::debug);

            s_logger->flush_on(spdlog::level::info);
            s_logger->flush_on(spdlog::level::err);
            s_logger->flush_on(spdlog::level::critical);

            spdlog::flush_every(std::chrono::seconds(1));

        } catch (const std::exception &e) {
            throw std::runtime_error("Logger initialization failed: " + std::string(e.what()));
        }
    }

    std::shared_ptr<spdlog::logger>& Logger::get() {
        return s_logger;
    }

    std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;
} // namespace cppchat::server