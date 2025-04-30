//
// Created by baizeyv on 2025/4/30.
//

#include "TestMode.h"

#include <iostream>
#include <sstream>
#include <thread>

#include "Const.h"
#include "Helper.h"
#include "Solver.h"

TestMode::TestMode() : is_input(true), vita_test_solver(nullptr)
{
}

TestMode::~TestMode()
{
    delete arg_commands;
    delete commands;
    if (vita_test_solver)
    {
        vita_test_solver->stop();
    }
    if (vita_test_thread && vita_test_thread->joinable())
        vita_test_thread->join();
    delete vita_test_solver;
    if (vita_test_thread)
        vita_test_thread.reset();
}

void TestMode::setup()
{
    arg_commands = new std::map<std::string, std::function<void(const std::string&)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("vita", [this](const std::string& args)
    {
        if (!vita_test_thread_done && vita_test_thread && vita_test_solver)
        {
            std::cout << spd::VitaTestRunning << std::endl;
            return;
        }
        join(2); // # 终止上一个线程
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty())
        {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 136)
        {
            std::cout << spd::VitaTestLevelLengthException << std::endl;
            return;
        }
        std::cout << spd::VitaTestStart << params[0] << std::endl;
        vita_test_thread_done = false;
        this->vita_test_thread = std::make_unique<std::thread>(std::thread([params, this]()
        {
            vita_test_solver = new Solver(params[0]);
            vita_test_solver->test_dfs();
            vita_test_thread_done = true;
        }));
    }));

    arg_commands->insert(std::make_pair("query", [this](const std::string& args)
    {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty())
        {
            std::cout << spd::TaskTestArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita")
        {
            if (vita_test_solver)
            {
                if (vita_test_solver->solved)
                {
                    std::cout << spd::VitaTestSolved << vita_test_solver->calc << std::endl;
                    return;
                }
                else
                {
                    std::cout << spd::VitaTestSolving << vita_test_solver->calc << std::endl;
                    std::cout << *vita_test_solver->current_state << std::endl;
                    return;
                }
            }
            else
            {
                std::cout << spd::NoVitaTestTask << std::endl;
                return;
            }
        }
        else if (params[0] == "playvalve")
        {
            // TODO:
        }
        else
        {
            std::cout << spd::TaskTestOptionsException << std::endl;
            return;
        }
    }));

    arg_commands->insert(std::make_pair("stop", [this](const std::string& args)
    {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty())
        {
            std::cout << spd::StopTestArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita")
        {
            join(2);
        }
        else if (params[0] == "playvalve")
        {
            join(1);
        }
        else
        {
            std::cout << spd::StopTestOptionsException << std::endl;
            return;
        }
        // TODO:
    }));

    // #####################################################

    commands->insert(std::make_pair("exit", [this]()
    {
        join(0);
        is_input = false;
    }));
    commands->insert(std::make_pair("quit", [this]()
    {
        join(0);
        is_input = false;
    }));
    commands->insert(std::make_pair("clear", [this]()
    {
        system("cls");
        spd::output_icon();
    }));
    // TODO:
}

void TestMode::enter()
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

bool TestMode::input()
{
    return is_input;
}

void TestMode::join(int type)
{
    if (type == 0 || type == 2)
    {
        if (vita_test_thread_done && vita_test_thread && vita_test_thread->joinable())
        {
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_test_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
        }
        else if (!vita_test_thread_done && vita_test_thread)
        {
            if (vita_test_solver)
                vita_test_solver->stop();
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_test_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
            vita_test_thread.reset();
        }
    }
}
