//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace cppchat::api {
///
/// @brief Retrieves the value of an environment variable as a string.
///
/// If the variable is not found in the process environment, the specified
/// default value is returned instead.
///
/// Example usage:
/// @code
/// std::string host = get_env("HOST", "localhost");
/// @endcode
///
/// @param key Name of the environment variable to fetch.
/// @param default_val Fallback value if the variable is not set (default: empty
/// string).
/// @return The environment variable's value or the fallback value.
///
std::string get_env(const std::string &key,
                    const std::string &default_val = "");

///
/// @brief Retrieves the value of an environment variable as an integer.
///
/// Internally uses std::stoi to convert the variable's value. If the variable
/// is not found or the value is not a valid integer, the default is returned.
///
/// Example usage:
/// @code
/// int port = get_env_int("PORT", 8080);
/// @endcode
///
/// @param key Name of the environment variable to fetch.
/// @param default_val Fallback value if the variable is not set or invalid
/// (default: 0).
/// @return The integer value of the environment variable or the fallback.
///
int get_env_int(const std::string &key, int default_val = 0);
} // namespace cppchat::api

#endif // UTILS_H
