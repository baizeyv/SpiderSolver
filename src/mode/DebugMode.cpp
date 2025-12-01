//
// Created by baizeyv on 2025/5/1.
//

#include "DebugMode.h"

DebugMode::DebugMode() :
    is_input(true), vita_debug_solver(nullptr), playvalve_debug_solver(nullptr), pgmaker_debug_solver(nullptr),
    doodle_debug_solver(nullptr) {}

DebugMode::~DebugMode() {
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

    // # region pgmaker test
    if (pgmaker_debug_solver != nullptr)
        pgmaker_debug_solver->stop();
    if (pgmaker_debug_thread != nullptr && pgmaker_debug_thread->joinable())
        pgmaker_debug_thread->join();
    if (pgmaker_debug_solver != nullptr)
        delete pgmaker_debug_solver;
    if (pgmaker_debug_thread != nullptr)
        pgmaker_debug_thread.reset();

    // # region custom test
    if (doodle_debug_solver != nullptr)
        doodle_debug_solver->stop();
    if (doodle_debug_thread != nullptr && doodle_debug_thread->joinable())
        doodle_debug_thread->join();
    if (doodle_debug_solver != nullptr)
        delete doodle_debug_solver;
    if (doodle_debug_thread != nullptr)
        doodle_debug_thread.reset();
}

void DebugMode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("vita", [this](const std::string &args) {
        // # command: vita level_string
        if (!vita_debug_thread_done && vita_debug_thread != nullptr && vita_debug_solver != nullptr) {
            std::cout << spd::DebugVitaRunning << std::endl;
            return;
        }
        join(2); // # 终止上一个线程
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::DebugVitaArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 136) {
            std::cout << spd::DebugVitaLevelLengthException << std::endl;
            return;
        }
        std::cout << spd::DebugVitaStart << params[0] << std::endl;
        vita_debug_thread_done = false;
        this->vita_debug_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            vita_debug_solver = new Solver(params[0]);
            vita_debug_solver->call_test_dfs();
            vita_debug_thread_done = true;
            std::cout << spd::CompleteDesc << std::endl << "> ";
        }));
    }));
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string &args) {
        if (!playvalve_debug_thread_done && playvalve_debug_thread != nullptr && playvalve_debug_solver != nullptr) {
            std::cout << spd::DebugPlayValveRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::DebugPlayValveArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::DebugPlayValveArgumentsException << std::endl;
            return;
        }
        join(1); // # 终止上一个线程
        std::cout << spd::DebugPlayValveStart << seed << " " << suit << std::endl;
        playvalve_debug_thread_done = false;
        this->playvalve_debug_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            playvalve_debug_solver = new Solver(seed, suit);
            playvalve_debug_solver->call_test_dfs();
            playvalve_debug_thread_done = true;
            std::cout << spd::CompleteDesc << std::endl << "> ";
        }));
    }));
    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
        if (!pgmaker_debug_thread_done && pgmaker_debug_thread != nullptr && pgmaker_debug_solver != nullptr) {
            std::cout << spd::DebugPGMakerRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::DebugPGMakerArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::DebugPGMakerArgumentsException << std::endl;
            return;
        }
        join(3); // # 终止上一个线程
        std::cout << spd::DebugPGMakerStart << seed << " " << suit << std::endl;
        pgmaker_debug_thread_done = false;
        this->pgmaker_debug_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            pgmaker_debug_solver = new Solver(seed, suit, 13, true);
            pgmaker_debug_solver->call_test_dfs();
            pgmaker_debug_thread_done = true;
            std::cout << spd::CompleteDesc << std::endl << "> ";
        }));
    }));
    arg_commands->insert(std::make_pair("doodle", [this](const std::string &args) {
        // # command: custom level_string
        if (!doodle_debug_thread_done && doodle_debug_thread != nullptr && doodle_debug_solver != nullptr) {
            std::cout << spd::DebugDoodleRunning << std::endl;
            return;
        }
        join(4); // # 终止上一个线程
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::DebugDoodleArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 104) {
            std::cout << spd::DebugDoodleLevelLengthException << std::endl;
            return;
        }
        std::cout << spd::DebugDoodleStart << params[0] << std::endl;
        doodle_debug_thread_done = false;
        this->doodle_debug_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            doodle_debug_solver = new Solver(0, params[0]);
            doodle_debug_solver->call_test_dfs();
            doodle_debug_thread_done = true;
            std::cout << spd::CompleteDesc << std::endl << "> ";
        }));
    }));
    arg_commands->insert(std::make_pair("stop", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::DebugStopArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita") {
            join(2);
        } else if (params[0] == "playvalve") {
            join(1);
        } else if (params[0] == "pgmaker") {
            join(3);
        } else if (params[0] == "doodle") {
            join(4);
        } else {
            std::cout << spd::DebugStopOptionsException << std::endl;
            return;
        }
        Helper::trim_memory();
    }));
    commands->insert(std::make_pair("exit", [this]() {
        join(0);
        is_input = false;
    }));
    commands->insert(std::make_pair("quit", [this]() {
        join(0);
        is_input = false;
    }));
    commands->insert(std::make_pair("clear", []() {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []() { Helper::trim_memory(); }));
    commands->insert(std::make_pair("help", []() {
        std::cout << "You are in `debug mode (spider --debug)` now." << std::endl
                  << "Commands:" << std::endl
                  << std::endl
                  << "    Try to debug solve vita level, and output info to the debug_log path." << std::endl
                  << "        [[ vita `level_string` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m vita "
                     "YYVYWN,1;QXVUSO,1;RYNPWU,1;ZNTNSR,1;YRPZT,1;QTQNU,1;PZQOO,1;ZTSTN,1;PRVUR,1;OOSWX,1;"
                     "UWWQRXZRZYSPQXSVXWONTOSVVSRWWTZXPTPUYNQYUUXOVVPQZX,0 \033[0m]]<---"
                  << std::endl
                  << std::endl
                  << "    Try to debug solve play-valve level, and output info to the debug_log path." << std::endl
                  << "        [[ playvalve `level_seed` `suit_count` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m playvalve 12345 1 \033[0m]]<---" << std::endl
                  << std::endl
                  << "    Try to debug solve pg-maker level, and output info to the debug_log path." << std::endl
                  << "        [[ pgmaker `level_seed` `suit_count` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m pgmaker 12345 1 \033[0m]]<---" << std::endl
                  << std::endl
                  << "    Try to debug solve doodle level, and output info to the debug_log path." << std::endl
                  << "        [[ doodle `level_string` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m doodle "
                     "lkjihgedcballkjihcjihgfegcehifcjdklhacbbhdagicdgjbldmkmkjgljfaebdemfmkmamakbfgkedildbehfmccgebiah"
                     "ailffjm \033[0m]]<---"
                  << std::endl
                  << std::endl
                  << "    Stop the level currently being attempted to solve." << std::endl
                  << "        [[\033[32m stop `vita | playvalve | pgmaker | doodle` \033[0m]]" << std::endl
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

bool DebugMode::input() { return is_input; }

void DebugMode::join(const int type) {
    if (type == 0 || type == 2) {
        if (vita_debug_thread_done && vita_debug_thread != nullptr && vita_debug_thread->joinable()) {
            std::cout << spd::DebugVitaWaitThread << std::endl;
            vita_debug_thread->join();
            std::cout << spd::DebugVitaThreadEnd << std::endl;
        } else if (!vita_debug_thread_done && vita_debug_thread != nullptr) {
            if (vita_debug_solver)
                vita_debug_solver->stop();
            std::cout << spd::DebugVitaWaitThread << std::endl;
            vita_debug_thread->join();
            std::cout << spd::DebugVitaThreadEnd << std::endl;
            vita_debug_thread.reset();
        }
        if (vita_debug_solver != nullptr) {
            delete vita_debug_solver;
            vita_debug_solver = nullptr;
        }
    }
    if (type == 0 || type == 1) {
        if (playvalve_debug_thread_done && playvalve_debug_thread != nullptr && playvalve_debug_thread->joinable()) {
            std::cout << spd::DebugPlayValveWaitThread << std::endl;
            playvalve_debug_thread->join();
            std::cout << spd::DebugPlayValveThreadEnd << std::endl;
        } else if (!playvalve_debug_thread_done && playvalve_debug_thread != nullptr) {
            if (playvalve_debug_solver)
                playvalve_debug_solver->stop();
            std::cout << spd::DebugPlayValveWaitThread << std::endl;
            playvalve_debug_thread->join();
            std::cout << spd::DebugPlayValveThreadEnd << std::endl;
            playvalve_debug_thread.reset();
        }
        if (playvalve_debug_solver != nullptr) {
            delete playvalve_debug_solver;
            playvalve_debug_solver = nullptr;
        }
    }
    if (type == 0 || type == 3) {
        if (pgmaker_debug_thread_done && pgmaker_debug_thread != nullptr && pgmaker_debug_thread->joinable()) {
            std::cout << spd::DebugPGMakerWaitThread << std::endl;
            pgmaker_debug_thread->join();
            std::cout << spd::DebugPGMakerThreadEnd << std::endl;
        } else if (!pgmaker_debug_thread_done && pgmaker_debug_thread != nullptr) {
            if (pgmaker_debug_solver)
                pgmaker_debug_solver->stop();
            std::cout << spd::DebugPGMakerWaitThread << std::endl;
            pgmaker_debug_thread->join();
            std::cout << spd::DebugPGMakerThreadEnd << std::endl;
            pgmaker_debug_thread.reset();
        }
        if (pgmaker_debug_solver != nullptr) {
            delete pgmaker_debug_solver;
            pgmaker_debug_solver = nullptr;
        }
    }
    if (type == 0 || type == 4) {
        if (doodle_debug_thread_done && doodle_debug_thread != nullptr && doodle_debug_thread->joinable()) {
            std::cout << spd::DebugDoodleWaitThread << std::endl;
            doodle_debug_thread->join();
            std::cout << spd::DebugDoodleThreadEnd << std::endl;
        } else if (!doodle_debug_thread_done && doodle_debug_thread != nullptr) {
            if (doodle_debug_solver)
                doodle_debug_solver->stop();
            std::cout << spd::DebugDoodleWaitThread << std::endl;
            doodle_debug_thread->join();
            std::cout << spd::DebugDoodleThreadEnd << std::endl;
            doodle_debug_thread.reset();
        }
        if (doodle_debug_solver != nullptr) {
            delete doodle_debug_solver;
            doodle_debug_solver = nullptr;
        }
    }
}
