//
// Created by baizeyv on 2025/5/2.
//

#ifndef VITABEAN_H
#define VITABEAN_H
#include <map>
#include <string>
#include <vector>

#include "LevelBean.h"
#include "../json.hpp"


class VitaBean
{
public:
    std::map<std::string, std::vector<LevelBean>> data;
    friend void to_json(nlohmann::json& j, const VitaBean& v);
    friend void from_json(const nlohmann::json& j, VitaBean& v);
};


#endif //VITABEAN_H
