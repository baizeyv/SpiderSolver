//
// Created by baizeyv on 2025/5/4.
//

#ifndef LEVELBEAN2_H
#define LEVELBEAN2_H
#include <string>

#include "LevelBean.h"
#include "../json.hpp"


class LevelBean2 {
public:
    int id;
    std::string question;
    LevelBean convert() const;
    friend void to_json(nlohmann::json& j, const LevelBean2& v);
    friend void from_json(const nlohmann::json& j, LevelBean2& v);
};



#endif //LEVELBEAN2_H
