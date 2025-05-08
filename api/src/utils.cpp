//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/utils.h"

#include <iostream>

namespace cppchat::api {
    std::unordered_map<std::string, std::string> load_env(const std::string &filename) {
        std::unordered_map<std::string, std::string> env;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file " << filename << std::endl;
        }
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                env[key] = value;
            }
        }

        return env;
    }
} // cppchat::api
