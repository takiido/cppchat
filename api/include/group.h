//
// Created by Alex Tovshchyk on 2025-05-13.
//

#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <vector>
#include "json.hpp"

namespace cppchat::api {

using std::string, std::vector;

struct Group {
  string id;
  string name;
  string description;
  string owner;
  vector<string> members;
};

///
/// @brief helper function used for parsing group from json
/// @param j json object to parse from
/// @param g group structure to parse into
inline void to_json(nlohmann::json& j, const Group& g) {
  j = nlohmann::json{{"id", g.id},
                     {"name", g.name},
                     {"description", g.description},
                     {"owner", g.owner},
                     {"members", g.members}};
}

///
/// @brief helper function used for parsing group from json
/// @param j json object to parse from
/// @param g group structure to parse into
inline void from_json(const nlohmann::json& j, Group& g) {
  j.at("id").get_to(g.id);
  j.at("name").get_to(g.name);
  j.at("description").get_to(g.description);
  j.at("owner").get_to(g.owner);
  j.at("members").get_to(g.members);
}
}  // namespace cppchat::api
#endif  // GROUP_H
