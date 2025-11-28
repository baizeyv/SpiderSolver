//
// Created by baizeyv on 2025/5/4.
//

#include "GenMode.h"

GenMode::GenMode() : is_input(true), gen_solver(nullptr) {}

GenMode::~GenMode() {
    delete arg_commands;
    delete commands;
    // # region vita test
    if (gen_solver != nullptr) {
        gen_stop_flag = true;
        gen_solver->stop();
    }
    if (gen_thread != nullptr && gen_thread->joinable())
        gen_thread->join();
    if (gen_solver != nullptr)
        delete gen_solver;
    if (gen_thread != nullptr)
        gen_thread.reset();
}

void GenMode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("generate", [this](const std::string &args) {
        // # generate 3 E:\CK\Spider\gen 9999
        if (!gen_thread_done && gen_thread != nullptr && gen_solver != nullptr) {
            std::cout << spd::GenRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 3) {
            std::cout << spd::GenArgumentsException << std::endl;
            return;
        }
        int suit_count, step_limit;
        if (!Helper::try_parse_int(params[0], suit_count) || !Helper::try_parse_int(params[2], step_limit)) {
            std::cout << spd::GenArgumentsException << std::endl;
            return;
        }
        join(); // # 终止上一个线程
        gen_thread_done = false;
        this->gen_thread = std::make_unique<std::thread>(std::thread([params, suit_count, step_limit, this]() {
            const auto output = params[1] + "\\generation\\gen_" + Helper::get_current_timestamp_millis() + ".csv";
            int id = 0;
            while (true) {
                id++;
                std::random_device rd;
                std::seed_seq seed{rd(), rd(), rd(), rd(), rd()};
                std::mt19937 gen(seed);
                gen_solver = new Solver(gen(), suit_count);
                gen_solver->call_dfs(output, id, false, step_limit);
                if (gen_stop_flag)
                    break;
            }
            gen_stop_flag = false;
            gen_thread_done = true;
        }));
    }));
    // #####################################################
    commands->insert(std::make_pair("exit", [this]() {
        join();
        is_input = false;
    }));
    commands->insert(std::make_pair("quit", [this]() {
        join();
        is_input = false;
    }));
    commands->insert(std::make_pair("stop", [this]() {
        join();
        Helper::trim_memory();
    }));
    // commands->insert(std::make_pair(
    //         "memory", []() { std::cout << Helper::get_memory_usage() << "KB Memory Usage." << std::endl; }));
    commands->insert(std::make_pair("clear", []() {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []() { Helper::trim_memory(); }));
    commands->insert(std::make_pair("help", []() {
        std::cout << "You are in `GenMode(spider --generate)` now." << std::endl
                  << "Commands:" << std::endl
                  << "    Try to generate winning levels." << std::endl
                  << "        [[ generate `suit_count` `output_path` `step_limit` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m generate 1 E:/foo/bar/output 1000000 \033[0m]]<---" << std::endl
                  << std::endl
                  << "    Stop the generation currently being attempted to solve." << std::endl
                  << "        [[\033[32m stop \033[0m]]" << std::endl
                  << std::endl
                  << "    Clear screen." << std::endl
                  << "        [[\033[32m clear \033[0m]]" << std::endl
                  << std::endl
                  << "    Exit." << std::endl
                  << "        [[\033[32m exit \033[0m]]" << std::endl
                  << "        [[\033[32m quit \033[0m]]" << std::endl
                  << std::endl
                  << "    Trim memory." << std::endl
                  << "        [[\033[32m shrink \033[0m]]" << std::endl
                  << std::endl;
    }));
}

bool GenMode::input() { return is_input; }

void GenMode::join() {
    gen_stop_flag = true;
    if (gen_thread_done && gen_thread != nullptr && gen_thread->joinable()) {
        std::cout << spd::GenWaitThread << std::endl;
        gen_thread->join();
        std::cout << spd::GenThreadEnd << std::endl;
    } else if (!gen_thread_done && gen_thread != nullptr) {
        if (gen_solver)
            gen_solver->stop();
        std::cout << spd::GenWaitThread << std::endl;
        gen_thread->join();
        std::cout << spd::GenThreadEnd << std::endl;
        gen_thread.reset();
    }
    if (gen_solver != nullptr) {
        delete gen_solver;
        gen_solver = nullptr;
    }
    gen_stop_flag = false;
}
