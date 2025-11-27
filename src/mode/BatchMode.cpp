//
// Created by baizeyv on 2025/5/2.
//

#include "BatchMode.h"

#include "../json.hpp"
#include "../data/VitaBean.h"
#include "../data/VitaBean2.h"

BatchMode::BatchMode() : is_input(true), vita_batch_solver(nullptr), playvalve_batch_solver(nullptr),
                         pgmaker_batch_solver(nullptr), custom_batch_solver(nullptr) {
}

BatchMode::~BatchMode() {
    delete arg_commands;
    delete commands;
    // # region vita test
    if (vita_batch_solver != nullptr) {
        vita_batch_stop_flag = true;
        vita_batch_solver->stop();
    }
    if (vita_batch_thread != nullptr && vita_batch_thread->joinable())
        vita_batch_thread->join();
    if (vita_batch_solver != nullptr)
        delete vita_batch_solver;
    if (vita_batch_thread != nullptr)
        vita_batch_thread.reset();
    // # region playvalve test
    if (playvalve_batch_solver != nullptr) {
        playvalve_batch_stop_flag = true;
        playvalve_batch_solver->stop();
    }
    if (playvalve_batch_thread != nullptr && playvalve_batch_thread->joinable())
        playvalve_batch_thread->join();
    if (playvalve_batch_solver != nullptr)
        delete playvalve_batch_solver;
    if (playvalve_batch_thread != nullptr)
        playvalve_batch_thread.reset();

    // # region pgmaker test
    if (pgmaker_batch_solver != nullptr) {
        pgmaker_batch_stop_flag = true;
        pgmaker_batch_solver->stop();
    }
    if (pgmaker_batch_thread != nullptr && pgmaker_batch_thread->joinable())
        pgmaker_batch_thread->join();
    if (pgmaker_batch_solver != nullptr)
        delete pgmaker_batch_solver;
    if (pgmaker_batch_thread != nullptr)
        pgmaker_batch_thread.reset();

    // # region custom test
    if (custom_batch_solver != nullptr) {
        custom_batch_stop_flag = true;
        custom_batch_solver->stop();
    }
    if (custom_batch_thread != nullptr && custom_batch_thread->joinable())
        custom_batch_thread->join();
    if (custom_batch_solver != nullptr)
        delete custom_batch_solver;
    if (custom_batch_thread != nullptr)
        custom_batch_thread.reset();
}

void BatchMode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)> >;
    commands = new std::map<std::string, std::function<void()> >;
    // vita C:\foo\bar\level.json C:\foo\bar\output 1000000
    // playvalve C:\foo\bar\level.txt C:\foo\bar\output  1 1000000
    // pgmaker C:\foo\bar\level.txt C:\foo\bar\output  1 1000000
    arg_commands->insert(std::make_pair("vita", [this](const std::string &args) {
        if (!vita_batch_thread_done && vita_batch_thread != nullptr && vita_batch_solver != nullptr) {
            std::cout << spd::VitaTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 3 && params.size() != 2) {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        auto json_path = params[0];
        auto output_path = params[1];
        auto json_content = Helper::read_file(json_path);
        const nlohmann::json j = nlohmann::json::parse(json_content);
        int step_limit = -1;
        if (params.size() == 3) {
            if (!Helper::try_parse_int(params[2], step_limit)) {
                std::cout << spd::VitaTestArgumentsException << std::endl;
                return;
            }
        }
        join(2); // # 终止上一个线程
        vita_batch_thread_done = false;
        try {
            auto vita_bean = j.get<VitaBean>();
            this->vita_batch_thread = std::make_unique<std::thread>(std::thread(
                [params, vita_bean, step_limit, this]() {
                    const auto output = params[1] + "\\vita\\vita_" + Helper::get_current_timestamp_millis() + ".csv";
                    for (auto &item: vita_bean.data) {
                        for (auto &level: item.second) {
                            vita_batch_solver = new Solver(level.question);
                            if (int id; Helper::try_parse_int(level.id, id)) {
                                vita_batch_solver->call_dfs(output, id, true, step_limit);
                            }
                            if (vita_batch_stop_flag) {
                                break;
                            }
                        }
                        if (vita_batch_stop_flag) {
                            break;
                        }
                    }
                    vita_batch_stop_flag = false;
                    vita_batch_thread_done = true;
                }));
        } catch (...) {
            auto vita_bean = j.get<VitaBean2>();
            this->vita_batch_thread = std::make_unique<std::thread>(std::thread(
                [params, vita_bean, step_limit, this]() {
                    const auto output = params[1] + "\\vita\\vita_" + Helper::get_current_timestamp_millis() + ".csv";
                    for (auto &item: vita_bean.data) {
                        for (auto &level: item.second) {
                            vita_batch_solver = new Solver(level.question);
                            const int id = level.id;
                            vita_batch_solver->call_dfs(output, id, true, step_limit);
                            if (vita_batch_stop_flag) {
                                break;
                            }
                        }
                        if (vita_batch_stop_flag) {
                            break;
                        }
                    }
                    vita_batch_stop_flag = false;
                    vita_batch_thread_done = true;
                }));
        }
    }));
    arg_commands->insert(std::make_pair("playvalve", [this](const std::string &args) {
        if (!playvalve_batch_thread_done && playvalve_batch_thread != nullptr && playvalve_batch_solver != nullptr) {
            std::cout << spd::PlayValveTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 4 && params.size() != 3) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        auto txt_path = params[0];
        auto output_path = params[1];
        auto txt_content = Helper::read_file(txt_path);
        auto seeds = Helper::split(txt_content, ",");
        int step_limit = -1;
        int suit_count = 1;
        if (params.size() == 4) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 3) {
            if (!Helper::try_parse_int(params[2], suit_count)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }
        join(1); // # 终止上一个线程
        playvalve_batch_thread_done = false;
        this->playvalve_batch_thread = std::make_unique<std::thread>(std::thread(
            [params, seeds, suit_count, step_limit, this]() {
                const auto output = params[1] + "\\playvalve\\playvalve_" + Helper::get_current_timestamp_millis() +
                                    ".csv";
                int id = 0;
                for (auto &item: seeds) {
                    id++;
                    if (int seed; Helper::try_parse_int(item, seed)) {
                        playvalve_batch_solver = new Solver(seed, suit_count);
                        playvalve_batch_solver->call_dfs(output, id, true, step_limit);
                    }
                    if (playvalve_batch_stop_flag) {
                        break;
                    }
                }
                playvalve_batch_stop_flag = false;
                playvalve_batch_thread_done = true;
            }));
    }));

    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
        if (!pgmaker_batch_thread_done && pgmaker_batch_thread != nullptr && pgmaker_batch_solver != nullptr) {
            std::cout << spd::PlayValveTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 4 && params.size() != 3) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        auto txt_path = params[0];
        auto output_path = params[1];
        auto txt_content = Helper::read_file(txt_path);
        auto seeds = Helper::split(txt_content, ",");
        int step_limit = -1;
        int suit_count = 1;
        if (params.size() == 4) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 3) {
            if (!Helper::try_parse_int(params[2], suit_count)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }
        join(3); // # 终止上一个线程
        pgmaker_batch_thread_done = false;
        this->pgmaker_batch_thread = std::make_unique<std::thread>(std::thread(
            [params, seeds, suit_count, step_limit, this]() {
                const auto output = params[1] + "\\pgmaker\\pgmaker_" + Helper::get_current_timestamp_millis() + ".csv";
                int id = 0;
                for (auto &item: seeds) {
                    id++;
                    if (int seed; Helper::try_parse_int(item, seed)) {
                        pgmaker_batch_solver = new Solver(seed, suit_count, 13, true);
                        pgmaker_batch_solver->call_dfs(output, id, true, step_limit);
                    }
                    if (pgmaker_batch_stop_flag) {
                        break;
                    }
                }
                pgmaker_batch_stop_flag = false;
                pgmaker_batch_thread_done = true;
            }));
    }));

    arg_commands->insert(std::make_pair("pgmakerskip", [this](const std::string &args) {
        if (!pgmaker_batch_thread_done && pgmaker_batch_thread != nullptr && pgmaker_batch_solver != nullptr) {
            std::cout << spd::PlayValveTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 5 && params.size() != 4) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        const auto txt_path = params[0];
        auto output_path = params[1];
        const auto txt_content = Helper::read_file(txt_path);
        auto seeds = Helper::split(txt_content, ",");
        int step_limit = -1;
        int suit_count = 1;
        int seed_limit = 0;
        if (params.size() == 5) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit) || !
                Helper::try_parse_int(params[4], seed_limit)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 4) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], seed_limit)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }
        join(3); // # 终止上一个线程
        pgmaker_batch_thread_done = false;
        this->pgmaker_batch_thread = std::make_unique<std::thread>(std::thread(
            [params, seeds, suit_count, step_limit, seed_limit, this]() {
                const auto output = params[1] + "\\pgmaker\\pgmaker_" + Helper::get_current_timestamp_millis() + ".csv";
                int id = 0;
                bool can_execute = false;
                for (auto &item: seeds) {
                    id++;
                    if (int seed; Helper::try_parse_int(item, seed)) {
                        if (seed == seed_limit) {
                            can_execute = true;
                            continue;
                        }
                        if (!can_execute)
                            continue;
                        pgmaker_batch_solver = new Solver(seed, suit_count, 13, true);
                        pgmaker_batch_solver->call_dfs(output, id, true, step_limit);
                    }
                    if (pgmaker_batch_stop_flag) {
                        break;
                    }
                }
                pgmaker_batch_stop_flag = false;
                pgmaker_batch_thread_done = true;
            }));
    }));

    // ! custom input_csv_path output_csv_path suit_count step_limit seed_limit
    arg_commands->insert(std::make_pair("custom", [this](const std::string &args) {
        // # 传入一个csv文件,这个文件中不能有标题,第一列是id, 第二列是104字符题目信息

        if (!custom_batch_thread_done && custom_batch_thread != nullptr && custom_batch_solver != nullptr) {
            std::cout << spd::PlayValveTestRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 5 && params.size() != 4) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }

        // # 输入的csv文件的绝对路径
        const std::string csv_path = params[0];
        // # 输出的csv文件的绝对路径
        std::string output_path = params[1];
        auto csv_content = Helper::read_file_line(csv_path);

        int step_limit = -1;
        int suit_count = 1;
        int seed_limit = -1;
        if (params.size() == 5) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit) || !
                Helper::try_parse_int(params[4], seed_limit)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 4) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }

        join(4); // # 终止上一个线程
        custom_batch_thread_done = false;
        this->custom_batch_thread = std::make_unique<std::thread>(std::thread(
            [params, csv_content, step_limit, seed_limit, suit_count, this]() {
                const auto output = params[1] + "\\custom\\custom_" + Helper::get_current_timestamp_millis() + ".csv";
                int real_id = 0;

                bool can_execute = false;
                for (auto item: csv_content) {
                    real_id++;
                    auto info = Helper::split(item, ",");
                    int id;
                    Helper::try_parse_int(info[0], id);

                    if (seed_limit >= 0) {
                        if (id == seed_limit) {
                            can_execute = true;
                            continue;
                        }
                        if (!can_execute)
                            continue;
                    }

                    auto str104 = info[1];
                    custom_batch_solver = new Solver(id, str104, suit_count);
                    custom_batch_solver->call_dfs(output, real_id, true, step_limit);
                    if (custom_batch_stop_flag) {
                        break;
                    }
                }
                custom_batch_stop_flag = false;
                custom_batch_thread_done = true;
            }));
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
    commands->insert(std::make_pair("shrink", []() {
        Helper::trim_memory();
    }));
    commands->insert(std::make_pair("query", [this]() {
        if (vita_batch_solver != nullptr) {
            if (vita_batch_solver->solved) {
                std::cout << spd::VitaTestSolved << vita_batch_solver->poker->mark << " ->CALC:" << vita_batch_solver->
                        calc << std::endl;
                return;
            } else {
                std::cout << spd::VitaTestSolving << vita_batch_solver->poker->mark << " ->CALC:" << vita_batch_solver->
                        calc << std::endl;
                return;
            }
        }
        if (playvalve_batch_solver != nullptr) {
            if (playvalve_batch_solver->solved) {
                std::cout << spd::PlayValveTestSolved << playvalve_batch_solver->poker->mark << " ->CALC:" <<
                        playvalve_batch_solver->calc << std::endl;
                return;
            } else {
                std::cout << spd::PlayValveTestSolving << playvalve_batch_solver->poker->mark << " ->CALC:" <<
                        playvalve_batch_solver->calc << std::endl;
                return;
            }
        }
        if (pgmaker_batch_solver != nullptr) {
            if (pgmaker_batch_solver->solved) {
                std::cout << spd::PlayValveTestSolved << pgmaker_batch_solver->poker->mark << " ->CALC:" <<
                        pgmaker_batch_solver->calc << std::endl;
                return;
            } else {
                std::cout << spd::PlayValveTestSolving << pgmaker_batch_solver->poker->mark << " ->CALC:" <<
                        pgmaker_batch_solver->calc << std::endl;
                return;
            }
        }
    }));
    commands->insert(std::make_pair("help", [this]() {
        std::cout << "You are in `BatchMode(spider --batch)` now." << std::endl
                << "Commands:" << std::endl
                << "    vita `level_json_file_path` `output_path` (`step_limit`) -> Try to solve the Vita level." <<
                std::endl
                << "    playvalve `level_txt_file_path` `output_path` `suit_count` (`step_limit`) -> Try to solve the PlayValve level."
                << std::endl
                << "    stop `vita | playvalve` -> Stop the level currently being attempted to solve." << std::endl
                << "    query -> Query Vita and PlayValve Exporter." << std::endl
                << "    shrink -> Trim memory." << std::endl
                << "    custom input_csv_path output_csv_path suit_count step_limit seed_limit" << std::endl;
    }));
}

bool BatchMode::input() {
    return is_input;
}

void BatchMode::join(const int type) {
    if (type == 0 || type == 2) {
        vita_batch_stop_flag = true;
        if (vita_batch_thread_done && vita_batch_thread != nullptr && vita_batch_thread->joinable()) {
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_batch_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
        } else if (!vita_batch_thread_done && vita_batch_thread != nullptr) {
            if (vita_batch_solver != nullptr)
                vita_batch_solver->stop();
            std::cout << spd::VitaTestWaitThread << std::endl;
            vita_batch_thread->join();
            std::cout << spd::VitaTestThreadEnd << std::endl;
            vita_batch_thread.reset();
        }
        if (vita_batch_solver != nullptr) {
            delete vita_batch_solver;
            vita_batch_solver = nullptr;
        }
        vita_batch_stop_flag = false;
    }
    if (type == 0 || type == 1) {
        playvalve_batch_stop_flag = true;
        if (playvalve_batch_thread_done && playvalve_batch_thread != nullptr && playvalve_batch_thread->joinable()) {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        } else if (!playvalve_batch_thread_done && playvalve_batch_thread != nullptr) {
            if (playvalve_batch_solver != nullptr)
                playvalve_batch_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            playvalve_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            playvalve_batch_thread.reset();
        }
        if (playvalve_batch_solver != nullptr) {
            delete playvalve_batch_solver;
            playvalve_batch_solver = nullptr;
        }
        playvalve_batch_stop_flag = false;
    }
    if (type == 0 || type == 3) {
        pgmaker_batch_stop_flag = true;
        if (pgmaker_batch_thread_done && pgmaker_batch_thread != nullptr && pgmaker_batch_thread->joinable()) {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            pgmaker_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        } else if (!pgmaker_batch_thread_done && pgmaker_batch_thread != nullptr) {
            if (pgmaker_batch_solver != nullptr)
                pgmaker_batch_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            pgmaker_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            pgmaker_batch_thread.reset();
        }
        if (pgmaker_batch_solver != nullptr) {
            delete pgmaker_batch_solver;
            pgmaker_batch_solver = nullptr;
        }
        pgmaker_batch_stop_flag = false;
    }
    // # custom
    if (type == 0 || type == 4) {
        custom_batch_stop_flag = true;
        if (custom_batch_thread_done && custom_batch_thread != nullptr && custom_batch_thread->joinable()) {
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            custom_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
        } else if (!custom_batch_thread_done && custom_batch_thread != nullptr) {
            if (custom_batch_solver != nullptr)
                custom_batch_solver->stop();
            std::cout << spd::PlayValveTestWaitThread << std::endl;
            custom_batch_thread->join();
            std::cout << spd::PlayValveTestThreadEnd << std::endl;
            custom_batch_thread.reset();
        }
        if (custom_batch_solver != nullptr) {
            delete custom_batch_solver;
            custom_batch_solver = nullptr;
        }
        custom_batch_stop_flag = false;
    }
}
