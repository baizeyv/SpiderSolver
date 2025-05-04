//
// Created by baizeyv on 2025/5/4.
//

#include "VitaBean2.h"

void to_json(nlohmann::json& j, const VitaBean2& v)
{
    j = nlohmann::json::object();
    for (const auto& [fst, snd] : v.data)
    {
        j[fst] = snd;
    }
}

void from_json(const nlohmann::json& j, VitaBean2& v)
{
    for (const auto& el : j.items())
    {
        v.data[el.key()] = el.value().get<std::vector<LevelBean2>>();
    }
}

VitaBean VitaBean2::convert() const
{
    VitaBean bean;
    for (const auto& [ik, ikv] : data)
    {
        std::vector<LevelBean> vec;
        for (const auto& x : ikv)
        {
            vec.push_back(x.convert());
        }
        bean.data.insert(make_pair(ik, vec));
    }
    return bean;
}
