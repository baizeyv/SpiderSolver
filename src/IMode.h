//
// Created by baizeyv on 2025/4/30.
//

#ifndef IMODE_H
#define IMODE_H
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "Const.h"
#include "Helper.h"

class IMode
{
public:
    virtual ~IMode() = default;
    
    std::map<std::string, std::function<void(const std::string&)>>* arg_commands;
    
    std::map<std::string, std::function<void()>>* commands;
    
    virtual void setup() = 0;

    virtual void enter()
    {
        std::string input_content;
        while (input())
        {
            std::cout << "> " << std::flush;
            std::getline(std::cin, input_content);
            if (input_content.empty())
                // # 输入内容为空
                    continue;

            // # 使用stringstream 解析输入的命令和参数
            std::istringstream stream(input_content);
            std::string command;
            stream >> command; // # 提取命令部分
            Helper::ltrim(command);
            if (command.empty())
                continue;

            // std::cout << "command->" << command << std::endl;

            std::string arguments;
            std::getline(stream, arguments); // # 获取命令之后的所有部分
            Helper::ltrim(arguments);

            // std::cout << "arguments->" << arguments << std::endl;
            if (arguments.empty())
            {
                // # 没有参数
                if (commands->contains(command))
                {
                    commands->at(command)(); // # 调用命令函数
                }
                else
                {
                    std::cout << spd::UnknowCommand << command << std::endl;
                }
            }
            else
            {
                if (arg_commands->contains(command))
                {
                    arg_commands->at(command)(arguments); // # 调用命令函数,传递参数
                }
                else
                {
                    std::cout << spd::UnknowCommand << command << std::endl;
                }
            }
        }
    }

    virtual bool input() = 0;
};

#endif //IMODE_H
