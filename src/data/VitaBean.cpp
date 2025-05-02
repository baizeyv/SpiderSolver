//
// Created by baizeyv on 2025/5/2.
//

#include "VitaBean.h"

void to_json(nlohmann::json& j, const VitaBean& v)
{
    j = nlohmann::json::object();
    for (const auto& [fst, snd] : v.data)
    {
        j[fst] = snd;
    }
}

void from_json(const nlohmann::json& j, VitaBean& v)
{
    for (const auto& el : j.items())
    {
        v.data[el.key()] = el.value().get<std::vector<LevelBean>>();
    }
}
