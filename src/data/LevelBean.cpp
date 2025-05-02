//
// Created by baizeyv on 2025/5/2.
//

#include "LevelBean.h"

void to_json(nlohmann::json& j, const LevelBean& v)
{
    j = nlohmann::json{{"id", v.id}, {"question", v.question}};
}

void from_json(const nlohmann::json& j, LevelBean& v)
{
    j.at("id").get_to(v.id);
    j.at("question").get_to(v.question);
}
