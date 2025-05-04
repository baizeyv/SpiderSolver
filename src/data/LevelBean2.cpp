//
// Created by baizeyv on 2025/5/4.
//

#include "LevelBean2.h"

void to_json(nlohmann::json& j, const LevelBean2& v)
{
    j = nlohmann::json{{"id", std::to_string(v.id)}, {"question", v.question}};
}

void from_json(const nlohmann::json& j, LevelBean2& v)
{
    j.at("id").get_to(v.id);
    j.at("question").get_to(v.question);
}

LevelBean LevelBean2::convert() const
{
    LevelBean bean;
    bean.id = std::to_string(id);
    bean.question = question;
    return bean;
}
