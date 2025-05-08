//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

namespace cppchat::api {
    ///
    /// @brief looks complex but only used to read .env
    /// @param filename path to .env file
    /// @return surprisingly .env values
    std::unordered_map<std::string, std::string> load_env(const std::string &filename);
} // cppchat::api

#endif //UTILS_H
