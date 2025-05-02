//
// Created by baizeyv on 2025/5/2.
//

#include "BatchMode.h"

#include "../json.hpp"
#include "../data/VitaBean.h"

BatchMode::BatchMode() : is_input(true), vita_batch_solver(nullptr), playvalve_batch_solver(nullptr)
{
}

BatchMode::~BatchMode()
{
    delete arg_commands;
    delete commands;
    // # region vita test
    if (vita_batch_solver)
    {
        vita_batch_stop_flag = true;
        vita_batch_solver->stop();
    }
    if (vita_batch_thread && vita_batch_thread->joinable())
        vita_batch_thread->join();
    delete vita_batch_solver;
    if (vita_batch_thread)
        vita_batch_thread.reset();
    // # region playvalve test
    if (playvalve_batch_solver)
    {
        playvalve_batch_stop_flag = true;
        playvalve_batch_solver->stop();
    }
    if (playvalve_batch_thread && playvalve_batch_thread->joinable())
        playvalve_batch_thread->join();
    delete playvalve_batch_solver;
    if (playvalve_batch_thread)
        playvalve_batch_thread.reset();
}

void BatchMode::setup()
{
    arg_commands = new std::map<std::string, std::function<void(const std::string&)>>;
    commands = new std::map<std::string, std::function<void()>>;
    // vita C:\foo\bar\level.json C:\foo\bar\output 1000000
    // playvalve C:\foo\bar\level.txt C:\foo\bar\output 1000000
    arg_commands->insert(std::make_pair("vita", [this](const std::string& args)
    {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 3 && params.size() != 2)
        {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        auto json_path = params[0];
        auto output_path = params[1];
        auto json_content = Helper::read_file(json_path);
        const nlohmann::json j = nlohmann::json::parse(json_content);
        auto vita_bean = j.get<VitaBean>();
        int step_limit = -1;
        if (params.size() == 3)
        {
            if (!Helper::try_parse_int(params[2], step_limit))
            {
                std::cout << spd::VitaTestArgumentsException << std::endl;
                return;
            }
        }
        join(2); // # 终止上一个线程
        vita_batch_thread_done = false;
        this->vita_batch_thread = std::make_unique<std::thread>(std::thread([params, vita_bean, step_limit, this]()
        {
            const auto output = params[1] + "\\vita\\vita_" + Helper::get_current_timestamp_millis() + ".csv";
            for (auto& item : vita_bean.data)
            {
                for (auto& level : item.second)
                {
                    vita_batch_solver = new Solver(level.question);
                    if (int id; Helper::try_parse_int(level.id, id))
                    {
                        vita_batch_solver->call_dfs(output, id, true, step_limit);
                    }
                    if (vita_batch_stop_flag)
                    {
                        break;
                    }
                }
                if (vita_batch_stop_flag)
                {
                    break;
                }
            }
            vita_batch_stop_flag = false;
            vita_batch_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string& args)
    {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 4 && params.size() != 3)
        {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        auto txt_path = params[0];
        auto output_path = params[1];
        auto txt_content = Helper::read_file(txt_path);
        auto seeds = Helper::split(txt_content, ",");
        int step_limit = -1;
        int suit_count = 1;
        if (params.size() == 4)
        {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit))
            {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }
        else if (params.size() == 3)
        {
            if (!Helper::try_parse_int(params[2], suit_count))
            {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }
        join(1); // # 终止上一个线程
        playvalve_batch_thread_done = false;
        this->playvalve_batch_thread = std::make_unique<std::thread>(std::thread([params, seeds, suit_count, step_limit, this]()
        {
            const auto output = params[1] + "\\playvalve\\playvalve_" + Helper::get_current_timestamp_millis() + ".csv";
            int id = 0;
            for (auto& item : seeds)
            {
                id++;
                if (int seed; Helper::try_parse_int(item, seed))
                {
                    playvalve_batch_solver = new Solver(seed, suit_count);
                    playvalve_batch_solver->call_dfs(output, id, true, step_limit);
                }
                if (playvalve_batch_stop_flag)
                {
                    break;
                }
            }
            playvalve_batch_stop_flag = false;
            playvalve_batch_thread_done = true;
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
}

bool BatchMode::input()
{
    return is_input;
}

void BatchMode::join(int type)
{
    if (type == 0 || type == 2)
    {
        vita_batch_stop_flag = true;
        if (vita_batch_thread_done && vita_batch_thread && vita_batch_thread->joinable())
        {
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_batch_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
        }
        else if (!vita_batch_thread_done && vita_batch_thread)
        {
            if (vita_batch_solver)
                vita_batch_solver->stop();
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_batch_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
            vita_batch_thread.reset();
        }
        vita_batch_stop_flag = false;
    }
    if (type == 0 || type == 1)
    {
        playvalve_batch_stop_flag = true;
        if (playvalve_batch_thread_done && playvalve_batch_thread && playvalve_batch_thread->joinable())
        {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        }
        else if (!playvalve_batch_thread_done && playvalve_batch_thread)
        {
            if (playvalve_batch_solver)
                playvalve_batch_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            playvalve_batch_thread.reset();
        }
        playvalve_batch_stop_flag = false;
    }
}
