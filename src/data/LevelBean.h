//
// Created by baizeyv on 2025/5/2.
//

#ifndef LEVELBEAN_H
#define LEVELBEAN_H
#include <string>
#include "../json.hpp"


class LevelBean
{
public:
    std::string id;
    std::string question;
    friend void to_json(nlohmann::json& j, const LevelBean& v);
    friend void from_json(const nlohmann::json& j, LevelBean& v);
};


#endif //LEVELBEAN_H
