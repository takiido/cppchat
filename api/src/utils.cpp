//
// Created by Alex Tovshchyk on 2025-05-07.
//

#include "../include/utils.h"

#include <iostream>

namespace cppchat::api {
std::string get_env(const std::string &key,
                    const std::string &default_val) {
  const char *val = std::getenv(key.c_str());
  return val ? std::string(val) : default_val;
}

int get_env_int(const std::string &key, int default_val) {
  const char *val = std::getenv(key.c_str());
  if (!val)
    return default_val;
  try {
    return std::stoi(val);
  } catch (...) {
    return default_val;
  }
}
} // namespace cppchat::api
