//
// Created by baizeyv on 2025/5/1.
//

#include "DebugMode.h"

DebugMode::DebugMode() : is_input(true), vita_debug_solver(nullptr), playvalve_debug_solver(nullptr)
{
}

DebugMode::~DebugMode()
{
    delete arg_commands;
    delete commands;
    // # region vita test
    if (vita_debug_solver != nullptr)
        vita_debug_solver->stop();
    if (vita_debug_thread != nullptr && vita_debug_thread->joinable())
        vita_debug_thread->join();
    if (vita_debug_solver != nullptr)
        delete vita_debug_solver;
    if (vita_debug_thread != nullptr)
        vita_debug_thread.reset();
    
    // # region playvalve test
    if (playvalve_debug_solver != nullptr)
        playvalve_debug_solver->stop();
    if (playvalve_debug_thread != nullptr && playvalve_debug_thread->joinable())
        playvalve_debug_thread->join();
    if (playvalve_debug_solver != nullptr)
        delete playvalve_debug_solver;
    if (playvalve_debug_thread != nullptr)
        playvalve_debug_thread.reset();
}

void DebugMode::setup()
{
    arg_commands = new std::map<std::string, std::function<void(const std::string&)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("vita", [this](const std::string& args)
    {
        if (!vita_debug_thread_done && vita_debug_thread != nullptr && vita_debug_solver != nullptr)
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
        vita_debug_thread_done = false;
        this->vita_debug_thread = std::make_unique<std::thread>(std::thread([params, this]()
        {
            vita_debug_solver = new Solver(params[0]);
            vita_debug_solver->call_test_dfs();
            vita_debug_thread_done = true;
        }));
    }));
    
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string& args)
    {
        if (!playvalve_debug_thread_done && playvalve_debug_thread != nullptr && playvalve_debug_solver != nullptr)
        {
            std::cout << spd::PlayValveTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2)
        {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit))
        {
            std::cout << spd::PlayValveTestOptionsException << std::endl;
            return;
        }
        join(1); // # 终止上一个线程
        std::cout << spd::PlayValveTestStart << seed << " " << suit << std::endl;
        playvalve_debug_thread_done = false;
        this->playvalve_debug_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]()
        {
            playvalve_debug_solver = new Solver(seed, suit);
            playvalve_debug_solver->call_test_dfs();
            playvalve_debug_thread_done = true;
        }));
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
        Helper::trim_memory();
    }));

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
    commands->insert(std::make_pair("clear", []()
    {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []()
    {
        Helper::trim_memory();
    }));
    commands->insert(std::make_pair("help", [this]()
    {
        std::cout << "You are in `DebugMode(spider --debug)` now." << std::endl
            << "Commands:" << std::endl
            << "    vita `level_string` -> Try to solve the Vita level." << std::endl
            << "    playvalve `seed` `suit_count` -> Try to solve the PlayValve level." << std::endl
            << "    stop `vita | playvalve` -> Stop the level currently being attempted to solve." << std::endl
            << "    shrink -> Trim memory." << std::endl;
    }));
}

bool DebugMode::input()
{
    return is_input;
}

void DebugMode::join(const int type)
{
    if (type == 0 || type == 2)
    {
        if (vita_debug_thread_done && vita_debug_thread != nullptr && vita_debug_thread->joinable())
        {
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_debug_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
        }
        else if (!vita_debug_thread_done && vita_debug_thread != nullptr)
        {
            if (vita_debug_solver)
                vita_debug_solver->stop();
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_debug_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
            vita_debug_thread.reset();
        }
        if (vita_debug_solver != nullptr)
        {
            delete vita_debug_solver;
            vita_debug_solver = nullptr;
        }
    }
    if (type == 0 || type == 1)
    {
        if (playvalve_debug_thread_done && playvalve_debug_thread != nullptr && playvalve_debug_thread->joinable())
        {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_debug_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        }
        else if (!playvalve_debug_thread_done && playvalve_debug_thread != nullptr)
        {
            if (playvalve_debug_solver)
                playvalve_debug_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_debug_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            playvalve_debug_thread.reset();
        }
        if (playvalve_debug_solver != nullptr)
        {
            delete playvalve_debug_solver;
            playvalve_debug_solver = nullptr;
        }
    }
}
