//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

using std::string, std::optional;

namespace cppchat::api {

    struct Message {
      string sender;
      optional<string> receiver;
      optional<string> group_id;
      string content;
      uint64_t timestamp;
    };
}
#endif //MESSAGE_H
