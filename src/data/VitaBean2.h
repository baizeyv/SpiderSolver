//
// Created by baizeyv on 2025/5/4.
//

#ifndef VITABEAN2_H
#define VITABEAN2_H
#include <map>
#include <string>
#include <vector>

#include "LevelBean2.h"
#include "VitaBean.h"
#include "../json.hpp"



class VitaBean2 {
public:
    std::map<std::string, std::vector<LevelBean2>> data;
    VitaBean convert() const;
    friend void to_json(nlohmann::json& j, const VitaBean2& v);
    friend void from_json(const nlohmann::json& j, VitaBean2& v);
};



#endif //VITABEAN2_H
