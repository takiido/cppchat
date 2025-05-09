//
// Created by Alex Tovshchyk on 2025-05-07.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include "json.hpp"

using std::string, std::optional, std::nullopt;

namespace cppchat::api {
    struct Message {
        string sender;
        optional<string> receiver;
        optional<string> group_id;
        string content;
        uint64_t timestamp;
    };

    ///
    /// @brief helper function used for parsing message to json
    /// @param j json object to parse into
    /// @param m message structure to parse from
    inline void to_json(nlohmann::json &j, const Message &m) {
        j = nlohmann::json{
            {"sender", m.sender},
            {"content", m.content},
            {"timestamp", m.timestamp}
        };

        if (m.receiver) j["receiver"] = m.receiver.value();
        if (m.group_id) j["group_id"] = m.group_id.value();
    }

    ///
    /// @brief helper function used for parsing message from json
    /// @param j json object to parse from
    /// @param m message structure to parse into
    inline void from_json(const nlohmann::json &j, Message &m) {
        j.at("sender").get_to(m.sender);

        if (j.contains("receiver") && !j["receiver"].is_null())
            m.receiver = j["receiver"].get<string>();
        else m.receiver = nullopt;

        if (j.contains("group_id") && !j["group_id"].is_null())
            m.group_id = j["group_id"].get<string>();
        else m.group_id = nullopt;

        j.at("content").get_to(m.content);
        j.at("timestamp").get_to(m.timestamp);
    }
}
#endif //MESSAGE_H
