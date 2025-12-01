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

TestMode::TestMode() :
    is_input(true), vita_test_solver(nullptr), playvalve_test_solver(nullptr), pgmaker_test_solver(nullptr),
    doodle_test_solver(nullptr) {}

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
    arg_commands = new std::map<std::string, std::function<void(const std::string &)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("vita", [this](const std::string &args) {
        if (!vita_test_thread_done && vita_test_thread != nullptr && vita_test_solver != nullptr) {
            std::cout << spd::TestVitaRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::TestVitaArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 136) {
            std::cout << spd::TestVitaLevelLengthException << std::endl;
            return;
        }
        join(2); // # 终止上一个线程
        std::cout << spd::TestVitaStart << params[0] << std::endl;
        vita_test_thread_done = false;
        this->vita_test_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            vita_test_solver = new Solver(params[0]);
            vita_test_solver->call_test_dfs();
            vita_test_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string &args) {
        if (!playvalve_test_thread_done && playvalve_test_thread != nullptr && playvalve_test_solver != nullptr) {
            std::cout << spd::TestPlayValveRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::TestPlayValveArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::TestPlayValveArgumentsException << std::endl;
            return;
        }
        join(1); // # 终止上一个线程
        std::cout << spd::TestPlayValveStart << seed << " " << suit << std::endl;
        playvalve_test_thread_done = false;
        this->playvalve_test_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            playvalve_test_solver = new Solver(seed, suit);
            playvalve_test_solver->call_test_dfs();
            playvalve_test_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
        if (!pgmaker_test_thread_done && pgmaker_test_thread != nullptr && pgmaker_test_solver != nullptr) {
            std::cout << spd::TestPGMakerRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::TestPGMakerArgumentsException << std::endl;
            return;
        }
        int seed, suit;
        if (!Helper::try_parse_int(params[0], seed) || !Helper::try_parse_int(params[1], suit)) {
            std::cout << spd::TestPGMakerArgumentsException << std::endl;
            return;
        }
        join(3); // # 终止上一个线程
        std::cout << spd::TestPGMakerStart << seed << " " << suit << std::endl;
        pgmaker_test_thread_done = false;
        this->pgmaker_test_thread = std::make_unique<std::thread>(std::thread([seed, suit, this]() {
            pgmaker_test_solver = new Solver(seed, suit, true);
            pgmaker_test_solver->call_test_dfs();
            pgmaker_test_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("doodle", [this](const std::string &args) {
        if (!doodle_test_thread_done && doodle_test_thread != nullptr && doodle_test_solver != nullptr) {
            std::cout << spd::TestDoodleRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::TestDoodleArgumentsException << std::endl;
            return;
        }
        if (params[0].length() != 104) {
            std::cout << spd::TestDoodleLevelLengthException << std::endl;
            return;
        }
        join(2); // # 终止上一个线程
        std::cout << spd::TestDoodleStart << params[0] << std::endl;
        doodle_test_thread_done = false;
        this->doodle_test_thread = std::make_unique<std::thread>(std::thread([params, this]() {
            doodle_test_solver = new Solver(params[0]);
            doodle_test_solver->call_test_dfs();
            doodle_test_thread_done = true;
        }));
    }));
    arg_commands->insert(std::make_pair("query", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::TestQueryArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita") {
            if (vita_test_solver != nullptr) {
                if (vita_test_solver->solved) {
                    std::cout << spd::TestVitaSolved << vita_test_solver->calc << std::endl;
                    return;
                } else {
                    std::cout << spd::TestVitaRunning << vita_test_solver->calc << std::endl;
                    vita_test_solver->prepare_query = 1;
                    // std::cout << *vita_test_solver->current_state << std::endl;
                    return;
                }
            } else {
                std::cout << spd::TestVitaNull << std::endl;
                return;
            }
        } else if (params[0] == "playvalve") {
            if (playvalve_test_solver != nullptr) {
                if (playvalve_test_solver->solved) {
                    std::cout << spd::TestPlayValveSolved << playvalve_test_solver->calc << std::endl;
                    return;
                } else {
                    std::cout << spd::TestPlayValveSolving << playvalve_test_solver->calc << std::endl;
                    playvalve_test_solver->prepare_query = 1;
                    // std::cout << *playvalve_test_solver->current_state << std::endl;
                    return;
                }
            } else {
                std::cout << spd::TestPlayValveNull << std::endl;
                return;
            }
        } else if (params[0] == "pgmaker") {
            if (pgmaker_test_solver != nullptr) {
                if (pgmaker_test_solver->solved) {
                    std::cout << spd::TestPGMakerSolved << pgmaker_test_solver->calc << std::endl;
                    return;
                } else {
                    std::cout << spd::TestPGMakerSolving << pgmaker_test_solver->calc << std::endl;
                    pgmaker_test_solver->prepare_query = 1;
                    return;
                }
            } else {
                std::cout << spd::TestPGMakerNull << std::endl;
                return;
            }
        } else if (params[0] == "doodle") {
            if (doodle_test_solver != nullptr) {
                if (doodle_test_solver->solved) {
                    std::cout << spd::TestDoodleSolved << doodle_test_solver->calc << std::endl;
                    return;
                } else {
                    std::cout << spd::TestDoodleSolving << doodle_test_solver->calc << std::endl;
                    doodle_test_solver->prepare_query = 1;
                    // std::cout << *doodle_test_solver->current_state << std::endl;
                    return;
                }
            } else {
                std::cout << spd::TestDoodleNull << std::endl;
                return;
            }
        } else {
            std::cout << spd::TestQueryOptionsException << std::endl;
            return;
        }
    }));
    arg_commands->insert(std::make_pair("stop", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::TestStopArgumentsException << std::endl;
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
            std::cout << spd::TestStopOptionsException << std::endl;
            return;
        }
    }));
    arg_commands->insert(std::make_pair("view", [](const std::string &args) {
        // # 查看起始牌型
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 3 || params.empty()) {
            std::cout << spd::TestViewArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita") {
            if (params.size() != 2) {
                std::cout << spd::TestViewArgumentsException << std::endl;
                return;
            }
            if (params[1].length() != 136) {
                std::cout << spd::TestViewVitaLevelLengthException << std::endl;
                return;
            } else {
                const auto poker = new Poker(params[1]);
                std::cout << *poker << std::endl;
                delete poker;
            }
        } else if (params[0] == "playvalve") {
            if (params.size() != 3) {
                std::cout << spd::TestViewArgumentsException << std::endl;
                return;
            }
            if (int seed, suit; Helper::try_parse_int(params[1], seed) && Helper::try_parse_int(params[2], suit)) {
                const auto poker = new Poker(seed, suit);
                std::cout << *poker << std::endl;
                delete poker;
            } else {
                std::cout << spd::TestViewArgumentsException << std::endl;
                return;
            }
        } else if (params[0] == "pgmaker-seed") // # 查看 pgmaker 的种子的关卡
        {
            if (params.size() != 3) {
                std::cout << spd::TestViewArgumentsException << std::endl;
                return;
            }
            if (int seed, suit; Helper::try_parse_int(params[1], seed) && Helper::try_parse_int(params[2], suit)) {
                const auto poker = new Poker(seed, suit, 13, true);
                std::cout << *poker << std::endl;
                delete poker;
            } else {
                std::cout << spd::TestViewArgumentsException << std::endl;
                return;
            }
        } else if (params[0] == "pgmaker-str") {
            // # 查看 pgmaker 的字符串的关卡
            if (params.size() != 3) {
                std::cout << spd::TestViewArgumentsException << std::endl;
                return;
            }

            const auto &lv_str = params[1];
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
            std::cout << spd::TestViewArgumentsException << std::endl;
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
    // commands->insert(std::make_pair(
    //         "memory", []() { std::cout << Helper::get_memory_usage() << "KB Memory Usage." << std::endl; }));
    commands->insert(std::make_pair("clear", []() {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []() { Helper::trim_memory(); }));
    commands->insert(std::make_pair("help", []() {
        std::cout << "You are in `test mode (spider --test)` now." << std::endl
                  << "Commands:" << std::endl
                  << std::endl
                  << "    Try to test solve vita level." << std::endl
                  << "        [[ vita `level_string` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m vita "
                     "YYVYWN,1;QXVUSO,1;RYNPWU,1;ZNTNSR,1;YRPZT,1;QTQNU,1;PZQOO,1;ZTSTN,1;PRVUR,1;OOSWX,1;"
                     "UWWQRXZRZYSPQXSVXWONTOSVVSRWWTZXPTPUYNQYUUXOVVPQZX,0 \033[0m]]<---"
                  << std::endl
                  << std::endl
                  << "    Try to test solve play-valve level." << std::endl
                  << "        [[ playvalve `level_seed` `suit_count` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m playvalve 12345 1 \033[0m]]<---" << std::endl
                  << std::endl
                  << "    Try to test solve pg-maker level." << std::endl
                  << "        [[ pgmaker `level_seed` `suit_count` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m pgmaker 12345 1 \033[0m]]<---" << std::endl
                  << std::endl
                  << "    Try to test solve doodle level." << std::endl
                  << "        [[ doodle `level_string` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m doodle "
                     "lkjihgedcballkjihcjihgfegcehifcjdklhacbbhdagicdgjbldmkmkjgljfaebdemfmkmamakbfgkedildbehfmccgebiah"
                     "ailffjm \033[0m]]<---"
                  << std::endl
                  << std::endl
                  << "    Query the level currently being attempted to solve." << std::endl
                  << "        [[\033[32m query `vita | playvalve | pgmaker | doodle` \033[0m]]" << std::endl
                  << std::endl
                  << "    View the level." << std::endl
                  << "        [[ view `vita args | playvalve args | pgmaker-seed args | pgmaker-str args | "
                     "doodle args` ]]"
                  << std::endl
                  << "            [[ view vita `level_string` ]]" << std::endl
                  << "            [[ view playvalve `level_seed` `level_suit` ]]" << std::endl
                  << "            [[ view pgmaker-seed `level_seed` `level_suit` ]]" << std::endl
                  << "            [[ view pgmaker-str `level_string` ]]" << std::endl
                  << "            [[ view doodle `level_string` ]]" << std::endl
                  << std::endl
                  << "        --->[[\033[32m view vita "
                     "YYVYWN,1;QXVUSO,1;RYNPWU,1;ZNTNSR,1;YRPZT,1;QTQNU,1;PZQOO,1;ZTSTN,1;PRVUR,1;OOSWX,1;"
                     "UWWQRXZRZYSPQXSVXWONTOSVVSRWWTZXPTPUYNQYUUXOVVPQZX,0 \033[0m]]<---"
                  << std::endl
                  << "        --->[[\033[32m view playvalve 12345 1 \033[0m]]<---" << std::endl
                  << "        --->[[\033[32m view pgmaker-seed 12345 1 \033[0m]]<---" << std::endl
                  << "        --->[[\033[32m view pgmaker-str "
                     "4,5,18,1,31,44,8,2,7,9,57,22,20,3,6,19,15,28,0,33,10,14,32,27,23,16,45,35,17,11,48,61,41,70,40,"
                     "21,54,24,67,58,53,13,34,66,37,47,12,79,83,71,25,26,39,50,38,51,52,64,60,65,29,77,42,30,55,43,46,"
                     "80,59,74,36,73,78,68,96,92,49,62,75,63,76,89,72,102,85,93,90,56,69,81,84,87,82,86,103,100,101,91,"
                     "88,99,98,97,95,94 \033[0m]]<---"
                  << std::endl
                  << "        --->[[\033[32m view doodle "
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


        // << "    query `vita | playvalve` -> Query the level currently being attempted to solve." << std::endl
        // << "    stop `vita | playvalve` -> Stop the level currently being attempted to solve." << std::endl
        // << "    view `vita` `vita_level_string` -> View the Vita level cards." << std::endl
        // << "    view `playvalve` `seed` `suit_count` -> View the PlayValve level cards." << std::endl
        // << "    view `pgmaker` `seed` `suit_count` -> View the PGMaker level cards." << std::endl
        // << "    shrink -> Trim memory." << std::endl;
    }));
}

bool TestMode::input() { return is_input; }

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
