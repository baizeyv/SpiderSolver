//
// Created by baizeyv on 2025/4/30.
//

#include "TestMode.h"

#include <csignal>
#include <iostream>
#include <sstream>
#include <thread>

#include "../Const.h"
#include "../Helper.h"
#include "../Solver.h"

TestMode::TestMode() : is_input(true), vita_test_solver(nullptr), playvalve_test_solver(nullptr),
                       pgmaker_test_solver(nullptr), doodle_test_solver(nullptr) {
}

TestMode::~TestMode() {
    delete arg_commands;
    delete commands;
    // # region vita test
    if (vita_test_solver != nullptr)
        vita_test_solver->stop();
    if (vita_test_thread != nullptr && vita_test_thread->joinable())
        vita_test_thread->join();
    if (vita_test_solver != nullptr)
        delete vita_test_solver;
    if (vita_test_thread != nullptr)
        vita_test_thread.reset();

    // # region playvalve test
    if (playvalve_test_solver != nullptr)
        playvalve_test_solver->stop();
    if (playvalve_test_thread != nullptr && playvalve_test_thread->joinable())
        playvalve_test_thread->join();
    if (playvalve_test_solver != nullptr)
        delete playvalve_test_solver;
    if (playvalve_test_thread != nullptr)
        playvalve_test_thread.reset();

    // # region pgmaker test
    if (pgmaker_test_solver != nullptr)
        pgmaker_test_solver->stop();
    if (pgmaker_test_thread != nullptr && pgmaker_test_thread->joinable())
        pgmaker_test_thread->join();
    if (pgmaker_test_solver != nullptr)
        delete pgmaker_test_solver;
    if (pgmaker_test_thread != nullptr)
        pgmaker_test_thread.reset();

    // # region doodle test
    if (doodle_test_solver != nullptr)
        doodle_test_solver->stop();
    if (doodle_test_thread != nullptr && doodle_test_thread->joinable())
        doodle_test_thread->join();
    if (doodle_test_solver != nullptr)
        delete doodle_test_solver;
    if (doodle_test_thread != nullptr)
        doodle_test_thread.reset();
}

void TestMode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)> >;
    commands = new std::map<std::string, std::function<void()> >;

    arg_commands->insert(std::make_pair("vita", [this](const std::string &args) {
        if (!vita_test_thread_done && vita_test_thread != nullptr && vita_test_solver != nullptr) {
            std::cout << spd::VitaTestRunning << std::endl;
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
        join(2); // # 终止上一个线程
        std::cout << spd::VitaTestStart << params[0] << std::endl;
        vita_test_thread_done = false;
        this->vita_test_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            vita_test_solver = new Solver(params[0]);
            vita_test_solver->call_test_dfs();
            vita_test_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string &args) {
        if (!playvalve_test_thread_done && playvalve_test_thread != nullptr && playvalve_test_solver != nullptr) {
            std::cout << spd::PlayValveTestRunning << std::endl;
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
        join(1); // # 终止上一个线程
        std::cout << spd::PlayValveTestStart << seed << " " << suit << std::endl;
        playvalve_test_thread_done = false;
        this->playvalve_test_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            playvalve_test_solver = new Solver(seed, suit);
            playvalve_test_solver->call_test_dfs();
            playvalve_test_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
        if (!pgmaker_test_thread_done && pgmaker_test_thread != nullptr && pgmaker_test_solver != nullptr) {
            std::cout << spd::PlayValveTestRunning << std::endl;
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
        join(3); // # 终止上一个线程
        std::cout << spd::PlayValveTestStart << seed << " " << suit << std::endl;
        pgmaker_test_thread_done = false;
        this->pgmaker_test_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            pgmaker_test_solver = new Solver(seed, suit, true);
            pgmaker_test_solver->call_test_dfs();
            pgmaker_test_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("query", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::TaskTestArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita") {
            if (vita_test_solver != nullptr) {
                if (vita_test_solver->solved) {
                    std::cout << spd::VitaTestSolved << vita_test_solver->calc << std::endl;
                    return;
                } else {
                    std::cout << spd::VitaTestSolving << vita_test_solver->calc << std::endl;
                    vita_test_solver->prepare_query = 1;
                    // std::cout << *vita_test_solver->current_state << std::endl;
                    return;
                }
            } else {
                std::cout << spd::NoVitaTestTask << std::endl;
                return;
            }
        } else if (params[0] == "playvalve") {
            if (playvalve_test_solver != nullptr) {
                if (playvalve_test_solver->solved) {
                    std::cout << spd::PlayValveTestSolved << playvalve_test_solver->calc << std::endl;
                    return;
                } else {
                    std::cout << spd::PlayValveTestSolving << playvalve_test_solver->calc << std::endl;
                    playvalve_test_solver->prepare_query = 1;
                    // std::cout << *playvalve_test_solver->current_state << std::endl;
                    return;
                }
            } else {
                std::cout << spd::NoPlayValveTestTask << std::endl;
                return;
            }
        } else if (params[0] == "pgmaker") {
            if (pgmaker_test_solver != nullptr) {
                if (pgmaker_test_solver->solved) {
                    std::cout << spd::PlayValveTestSolved << pgmaker_test_solver->calc << std::endl;
                    return;
                } else {
                    std::cout << spd::PlayValveTestSolving << pgmaker_test_solver->calc << std::endl;
                    pgmaker_test_solver->prepare_query = 1;
                    return;
                }
            } else {
                std::cout << spd::NoPlayValveTestTask << std::endl;
                return;
            }
        } else {
            std::cout << spd::TaskTestOptionsException << std::endl;
            return;
        }
    }));
    arg_commands->insert(std::make_pair("stop", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::StopTestArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita") {
            join(2);
        } else if (params[0] == "playvalve") {
            join(1);
        } else if (params[0] == "pgmaker") {
            join(3);
        } else {
            std::cout << spd::StopTestOptionsException << std::endl;
            return;
        }
    }));
    arg_commands->insert(std::make_pair("view", [](const std::string &args) {
        // # 查看起始牌型
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 3 || params.empty()) {
            std::cout << spd::ViewTestArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita") {
            if (params.size() != 2) {
                std::cout << spd::ViewTestArgumentsException << std::endl;
                return;
            }
            if (params[1].length() != 136) {
                std::cout << spd::ViewVitaLevelLengthException << std::endl;
                return;
            } else {
                const auto poker = new Poker(params[1]);
                std::cout << *poker << std::endl;
                delete poker;
            }
        } else if (params[0] == "playvalve") {
            if (params.size() != 3) {
                std::cout << spd::ViewTestArgumentsException << std::endl;
                return;
            }
            if (int seed, suit; Helper::try_parse_int(params[1], seed) && Helper::try_parse_int(params[2], suit)) {
                const auto poker = new Poker(seed, suit);
                std::cout << *poker << std::endl;
                delete poker;
            } else {
                std::cout << spd::ViewSpdSeedException << std::endl;
                return;
            }
        } else if (params[0] == "pgmaker") // # 查看 pgmaker 的种子的关卡
        {
            if (params.size() != 3) {
                std::cout << spd::ViewTestArgumentsException << std::endl;
                return;
            }
            if (int seed, suit; Helper::try_parse_int(params[1], seed) && Helper::try_parse_int(params[2], suit)) {
                const auto poker = new Poker(seed, suit, 13, true);
                std::cout << *poker << std::endl;
                delete poker;
            } else {
                std::cout << spd::ViewSpdSeedException << std::endl;
                return;
            }
        } else if (params[0] == "pgmakerspec") {
            // # 查看 pgmaker 的字符串的关卡
            if (params.size() != 3) {
                std::cout << spd::ViewTestArgumentsException << std::endl;
                return;
            }

            const auto lv_str = params[1];
            const auto array = Helper::split(lv_str, ",");
            std::vector<int> cds;
            for (const auto &item: array) {
                cds.push_back(std::stoi(item));
            }
            if (int suit = 1; Helper::try_parse_int(params[2], suit)) {
                const auto poker = new Poker(cds, suit);
                std::cout << *poker << std::endl;
                delete poker;
            }
        } else {
            std::cout << spd::ViewTestOptionsException << std::endl;
            return;
        }
        Helper::trim_memory();
    }));

    // #####################################################

    commands->insert(std::make_pair("exit", [this]() {
        join(0);
        is_input = false;
    }));
    commands->insert(std::make_pair("quit", [this]() {
        join(0);
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
        std::cout << "You are in `TestMode(spider --test)` now." << std::endl
                << "Commands:" << std::endl
                << "    vita `level_string` -> Try to solve the Vita level." << std::endl
                << "    playvalve `seed` `suit_count` -> Try to solve the PlayValve level." << std::endl
                << "    pgmaker `seed` `suit_count` -> Try to solve the PGMaker level." << std::endl
                << "    query `vita | playvalve` -> Query the level currently being attempted to solve." << std::endl
                << "    stop `vita | playvalve` -> Stop the level currently being attempted to solve." << std::endl
                << "    view `vita` `vita_level_string` -> View the Vita level cards." << std::endl
                << "    view `playvalve` `seed` `suit_count` -> View the PlayValve level cards." << std::endl
                << "    view `pgmaker` `seed` `suit_count` -> View the PGMaker level cards." << std::endl
                << "    shrink -> Trim memory." << std::endl;
    }));
}

bool TestMode::input() {
    return is_input;
}

void TestMode::join(const int type) {
    if (type == 0 || type == 2) {
        if (vita_test_thread_done && vita_test_thread != nullptr && vita_test_thread->joinable()) {
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_test_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
        } else if (!vita_test_thread_done && vita_test_thread != nullptr) {
            if (vita_test_solver)
                vita_test_solver->stop();
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_test_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
            vita_test_thread.reset();
        }
        if (vita_test_solver != nullptr) {
            delete vita_test_solver;
            vita_test_solver = nullptr;
        }
    }
    if (type == 0 || type == 1) {
        if (playvalve_test_thread_done && playvalve_test_thread != nullptr && playvalve_test_thread->joinable()) {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_test_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        } else if (!playvalve_test_thread_done && playvalve_test_thread != nullptr) {
            if (playvalve_test_solver)
                playvalve_test_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_test_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            playvalve_test_thread.reset();
        }
        if (playvalve_test_solver != nullptr) {
            delete playvalve_test_solver;
            playvalve_test_solver = nullptr;
        }
    }
    if (type == 0 || type == 3) {
        if (pgmaker_test_thread_done && pgmaker_test_thread != nullptr && pgmaker_test_thread->joinable()) {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            pgmaker_test_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        } else if (!pgmaker_test_thread_done && pgmaker_test_thread != nullptr) {
            if (pgmaker_test_solver)
                pgmaker_test_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            pgmaker_test_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            pgmaker_test_thread.reset();
        }
        if (pgmaker_test_solver != nullptr) {
            delete pgmaker_test_solver;
            pgmaker_test_solver = nullptr;
        }
    }
    if (type == 0 || type == 4) {
        if (doodle_test_thread_done && doodle_test_thread != nullptr && doodle_test_thread->joinable()) {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            doodle_test_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        } else if (!doodle_test_thread_done && doodle_test_thread != nullptr) {
            if (doodle_test_solver)
                doodle_test_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            doodle_test_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            doodle_test_thread.reset();
        }
        if (doodle_test_solver != nullptr) {
            delete doodle_test_solver;
            doodle_test_solver = nullptr;
        }
    }
}
