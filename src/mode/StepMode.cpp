//
// Created by baizeyv on 2025/5/4.
//

#include "StepMode.h"

StepMode::StepMode() : is_input(true), step_solver(nullptr) {}

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
    arg_commands = new std::map<std::string, std::function<void(const std::string &)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("vita", [this](const std::string &args) {
        if (!step_thread_done && step_thread != nullptr && step_solver != nullptr) {
            std::cout << spd::StepVitaRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::StepVitaArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 136) {
            std::cout << spd::StepVitaLevelLengthException << std::endl;
            return;
        }
        join();
        std::cout << spd::StepVitaStart << params[0] << std::endl;
        step_thread_done = false;
        this->step_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            step_solver = new Solver(params[0]);
            step_solver->call_step_dfs();
            step_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string &args) {
        if (!step_thread_done && step_thread != nullptr && step_solver != nullptr) {
            std::cout << spd::StepPlayValveRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::StepPlayValveArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::StepPlayValveArgumentsException << std::endl;
            return;
        }
        join();
        std::cout << spd::StepPlayValveStart << seed << " " << suit << std::endl;
        step_thread_done = false;
        this->step_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            step_solver = new Solver(seed, suit);
            step_solver->call_step_dfs();
            step_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
        if (!step_thread_done && step_thread != nullptr && step_solver != nullptr) {
            std::cout << spd::StepPGMakerRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::StepPGMakerArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::StepPGMakerArgumentsException << std::endl;
            return;
        }
        join();
        std::cout << spd::StepPGMakerStart << seed << " " << suit << std::endl;
        step_thread_done = false;
        this->step_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            step_solver = new Solver(seed, suit, 13, true);
            step_solver->call_step_dfs();
            step_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("doodle", [this](const std::string &args) {
        if (!step_thread_done && step_thread != nullptr && step_solver != nullptr) {
            std::cout << spd::StepDoodleRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::StepDoodleArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 104) {
            std::cout << spd::StepDoodleLevelLengthException << std::endl;
            return;
        }
        join();
        std::cout << spd::StepDoodleStart << params[0] << std::endl;
        step_thread_done = false;
        this->step_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            step_solver = new Solver(0, params[0]);
            step_solver->call_step_dfs();
            step_thread_done = true;
        }));
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
    // commands->insert(std::make_pair("memory", []() {
    //     std::cout << Helper::get_memory_usage() << "KB Memory Usage." << std::endl;
    // }));
    commands->insert(std::make_pair("clear", []() {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []() { Helper::trim_memory(); }));
    commands->insert(std::make_pair("help", []() {
        // TODO:
        std::cout << "You are in `step mode (spider --step)` now." << std::endl
                  << "Commands:" << std::endl
                  << std::endl


                  << "    vita `level_string` -> Try to solve the Vita level." << std::endl
                  << "    playvalve `seed` `suit_count` -> Try to solve the PlayValve level." << std::endl
                  << "    stop -> Stop the level currently being attempted to solve." << std::endl
                  << "    shrink -> Trim memory." << std::endl;
    }));
}

bool StepMode::input() { return is_input; }

void StepMode::join() {
    if (step_solver != nullptr)
        step_solver->abort_step = 1;
    if (step_thread_done && step_thread != nullptr && step_thread->joinable()) {
        std::cout << spd::StepWaitThread << std::endl;
        step_thread->join();
        std::cout << spd::StepThreadEnd << std::endl;
    } else if (!step_thread_done && step_thread != nullptr) {
        if (step_solver)
            step_solver->stop();
        std::cout << spd::StepWaitThread << std::endl;
        step_thread->join();
        std::cout << spd::StepThreadEnd << std::endl;
        step_thread.reset();
    }
    if (step_solver != nullptr) {
        delete step_solver;
        step_solver = nullptr;
    }
}
