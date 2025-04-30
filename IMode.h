//
// Created by baizeyv on 2025/4/30.
//

#ifndef IMODE_H
#define IMODE_H
#include <functional>
#include <map>
#include <string>

class IMode
{
public:
    virtual ~IMode() = default;
    
    std::map<std::string, std::function<void(const std::string&)>>* arg_commands;
    
    std::map<std::string, std::function<void()>>* commands;
    
    virtual void setup() = 0;

    virtual void enter() = 0;

    virtual bool input() = 0;
};

#endif //IMODE_H
