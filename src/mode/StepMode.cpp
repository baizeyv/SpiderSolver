//
// Created by baizeyv on 2025/5/4.
//

#include "StepMode.h"

StepMode::StepMode() : is_input(true), step_solver(nullptr) {
}

StepMode::~StepMode() {
    delete arg_commands;
    delete commands;
    if (step_solver != nullptr)
        step_solver->stop();
    if (step_thread != nullptr && step_thread->joinable())
        step_thread->join();
    if (step_solver != nullptr)
        delete step_solver;
    if (step_thread != nullptr)
        step_thread.reset();
}

void StepMode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)> >;
    commands = new std::map<std::string, std::function<void()> >;

    arg_commands->insert(std::make_pair("vita", [this](const std::string &args) {
        if (!step_thread_done && step_thread != nullptr && step_solver != nullptr) {
            std::cout << spd::StepTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 136) {
            std::cout << spd::VitaTestLevelLengthException << std::endl;
            return;
        }
        join();
        std::cout << spd::VitaTestStart << params[0] << std::endl;
        step_thread_done = false;
        this->step_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            step_solver = new Solver(params[0]);
            step_solver->call_step_dfs();
            step_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string &args) {
        if (!step_thread_done && step_thread != nullptr && step_solver != nullptr) {
            std::cout << spd::StepTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::PlayValveTestOptionsException << std::endl;
            return;
        }
        join();
        std::cout << spd::PlayValveTestStart << seed << " " << suit << std::endl;
        step_thread_done = false;
        this->step_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            step_solver = new Solver(seed, suit);
            step_solver->call_step_dfs();
            step_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
        if (!step_thread_done && step_thread != nullptr && step_solver != nullptr) {
            std::cout << spd::StepTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::PlayValveTestOptionsException << std::endl;
            return;
        }
        join();
        std::cout << spd::PlayValveTestStart << seed << " " << suit << std::endl;
        step_thread_done = false;
        this->step_thread = std::make_unique<std::thread>(std::thread(
            [seed, suit, this]() {
                step_solver = new Solver(seed, suit, 13, true);
                step_solver->call_step_dfs();
                step_thread_done = true;
            }
        ));
    }));
    commands->insert(std::make_pair("step", [this]() {
        if (step_solver != nullptr) {
            if (step_solver->solved) {
                std::cout << "Solved!" << std::endl;
                return;
            } else {
                step_solver->next_step = 1;
                return;
            }
        }
    }));
    commands->insert(std::make_pair("stop", [this]() {
        join();
        Helper::trim_memory();
    }));
    commands->insert(std::make_pair("exit", [this]() {
        join();
        is_input = false;
    }));
    commands->insert(std::make_pair("quit", [this]() {
        join();
        is_input = false;
    }));
    commands->insert(std::make_pair("memory", [this]() {
        std::cout << Helper::get_memory_usage() << "KB Memory Usage." << std::endl;
    }));
    commands->insert(std::make_pair("clear", []() {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []() {
        Helper::trim_memory();
    }));
    commands->insert(std::make_pair("help", [this]() {
        std::cout << "You are in `StepMode(spider --step)` now." << std::endl
                << "Commands:" << std::endl
                << "    vita `level_string` -> Try to solve the Vita level." << std::endl
                << "    playvalve `seed` `suit_count` -> Try to solve the PlayValve level." << std::endl
                << "    stop -> Stop the level currently being attempted to solve." << std::endl
                << "    shrink -> Trim memory." << std::endl;
    }));
}

bool StepMode::input() {
    return is_input;
}

void StepMode::join() {
    if (step_solver != nullptr)
        step_solver->abort_step = 1;
    if (step_thread_done && step_thread != nullptr && step_thread->joinable()) {
        std::cout << spd::VitaTestWaitThread << std::endl;
        step_thread->join();
        std::cout << spd::VitaTestThreadEnd << std::endl;
    } else if (!step_thread_done && step_thread != nullptr) {
        if (step_solver)
            step_solver->stop();
        std::cout << spd::VitaTestWaitThread << std::endl;
        step_thread->join();
        std::cout << spd::VitaTestThreadEnd << std::endl;
        step_thread.reset();
    }
    if (step_solver != nullptr) {
        delete step_solver;
        step_solver = nullptr;
    }
}
