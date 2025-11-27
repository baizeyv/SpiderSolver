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
            std::cout << spd::BatchVitaRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 3 && params.size() != 2) {
            std::cout << spd::BatchVitaArgumentsException << std::endl;
            return;
        }
        auto json_path = params[0];
        auto output_path = params[1];
        auto json_content = Helper::read_file(json_path);
        const nlohmann::json j = nlohmann::json::parse(json_content);
        int step_limit = -1;
        if (params.size() == 3) {
            if (!Helper::try_parse_int(params[2], step_limit)) {
                std::cout << spd::BatchVitaArgumentsException << std::endl;
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
            std::cout << spd::BatchPlayValveRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 4 && params.size() != 3) {
            std::cout << spd::BatchPlayValveArgumentsException << std::endl;
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
                std::cout << spd::BatchPlayValveArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 3) {
            if (!Helper::try_parse_int(params[2], suit_count)) {
                std::cout << spd::BatchPlayValveArgumentsException << std::endl;
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
            std::cout << spd::BatchPGMakerRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 5 && params.size() != 4 && params.size() != 3) {
            std::cout << spd::BatchPGMakerArgumentsException << std::endl;
            return;
        }
        const auto txt_path = params[0];
        auto output_path = params[1];
        const auto txt_content = Helper::read_file(txt_path);
        auto seeds = Helper::split(txt_content, ",");
        int step_limit = -1;
        int suit_count = 1;
        int seed_limit = -1;
        if (params.size() == 5) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit) || !
                Helper::try_parse_int(params[4], seed_limit)) {
                std::cout << spd::BatchPGMakerArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 4) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], seed_limit)) {
                std::cout << spd::BatchPGMakerArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 3) {
            if (!Helper::try_parse_int(params[2], suit_count)) {
                std::cout << spd::BatchPGMakerArgumentsException << std::endl;
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
                        if (seed_limit >= 0) {
                            if (seed == seed_limit) {
                                can_execute = true;
                                continue;
                            }
                            if (!can_execute)
                                continue;
                        }
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
    arg_commands->insert(std::make_pair("custom", [this](const std::string &args) {
        // ! custom input_csv_path output_csv_path suit_count step_limit seed_limit
        // # 传入一个csv文件,这个文件中不能有标题,第一列是id, 第二列是104字符题目信息

        if (!custom_batch_thread_done && custom_batch_thread != nullptr && custom_batch_solver != nullptr) {
            std::cout << spd::BatchCustomRunning << std::endl;
            return;
        }
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 5 && params.size() != 4 && params.size() != 3) {
            std::cout << spd::BatchCustomArgumentsException << std::endl;
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
                std::cout << spd::BatchCustomArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 4) {
            if (!Helper::try_parse_int(params[2], suit_count) || !Helper::try_parse_int(params[3], step_limit)) {
                std::cout << spd::BatchCustomArgumentsException << std::endl;
                return;
            }
        } else if (params.size() == 3) {
            if (!Helper::try_parse_int(params[2], suit_count)) {
                std::cout << spd::BatchCustomArgumentsException << std::endl;
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
                std::cout << "COMPLETED!!!" << std::endl << "> ";
            }));
    }));
    arg_commands->insert(std::make_pair("stop", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() > 1 || params.empty()) {
            std::cout << spd::BatchStopArgumentsException << std::endl;
            return;
        }
        if (params[0] == "vita") {
            join(2);
        } else if (params[0] == "playvalve") {
            join(1);
        } else if (params[0] == "pgmaker") {
            join(3);
        } else if (params[0] == "custom") {
            join(4);
        } else {
            std::cout << spd::BatchStopOptionsException << std::endl;
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
                std::cout << spd::BatchVitaSolved << vita_batch_solver->poker->mark << spd::Calc << vita_batch_solver->
                        calc << std::endl;
                return;
            } else {
                std::cout << spd::BatchVitaSolving << vita_batch_solver->poker->mark << spd::Calc << vita_batch_solver->
                        calc << std::endl;
                return;
            }
        }
        if (playvalve_batch_solver != nullptr) {
            if (playvalve_batch_solver->solved) {
                std::cout << spd::BatchPlayValveSolved << playvalve_batch_solver->poker->mark << spd::Calc <<
                        playvalve_batch_solver->calc << std::endl;
                return;
            } else {
                std::cout << spd::BatchPlayValveSolved << playvalve_batch_solver->poker->mark << spd::Calc <<
                        playvalve_batch_solver->calc << std::endl;
                return;
            }
        }
        if (pgmaker_batch_solver != nullptr) {
            if (pgmaker_batch_solver->solved) {
                std::cout << spd::BatchPGMakerSolved << pgmaker_batch_solver->poker->mark << spd::Calc <<
                        pgmaker_batch_solver->calc << std::endl;
                return;
            } else {
                std::cout << spd::BatchPGMakerSolving << pgmaker_batch_solver->poker->mark << spd::Calc <<
                        pgmaker_batch_solver->calc << std::endl;
                return;
            }
        }
        if (custom_batch_solver != nullptr) {
            if (custom_batch_solver->solved) {
                std::cout << spd::BatchCustomSolved << custom_batch_solver->poker->mark << spd::Calc <<
                        custom_batch_solver->calc << std::endl;
                return;
            } else {
                std::cout << spd::BatchCustomSolving << custom_batch_solver->poker->mark << spd::Calc <<
                        custom_batch_solver->calc << std::endl;
                return;
            }
        }
    }));
    commands->insert(std::make_pair("help", [this]() {
        std::cout << "You are in `batch mode(spider --batch)` now." << std::endl
                << "Commands:" << std::endl << std::endl
                << "    Try to batch solve vita levels." << std::endl
                << "        [[ vita `level_json_file_path` `output_path` (`step_limit`) ]]" << std::endl << std::endl
                << "        --->[[\033[32m vita E:/foo/bar/test.json E:/foo/bar/output 1000000 \033[0m]]<---" <<
                std::endl << std::endl
                << "    Try to batch solve play-valve levels." << std::endl
                << "        [[ playvalve `level_txt_file_path` `output_path` `suit_count` (`step_limit`) ]]" <<
                std::endl << std::endl
                << "        --->[[\033[32m playvalve E:/foo/bar/test.txt E:/foo/bar/output 1 1000000 \033[0m]]<---" <<
                std::endl <<
                std::endl
                << "    Try to batch solve pg-maker levels. (contains skip seed)" << std::endl
                << "        [[ pgmaker `level_txt_file_path` `output_path` `suit_count` (`step_limit` `seed_limit`) ]]"
                << std::endl
                << std::endl
                << "        --->[[\033[32m pgmaker E:/foo/bar/test.txt E:/foo/bar/output 1 1000000 7373 \033[0m]]<---"
                << std::endl <<
                std::endl
                << "    Try to batch solve custom levels (contains skip seed)." << std::endl
                << "        [[ custom `level_csv_file_path` `output_path` `suit_count` (`step_limit` `seed_limit`) ]]"
                << std::endl << std::endl
                << "        --->[[\033[32m custom E:/foobar/test.csv E:/foo/bar/output 1 1000000 7373 \033[0m]]<---" <<
                std::endl <<
                std::endl
                << "    Stop the level currently being attempted to solve." << std::endl
                << "        [[\033[32m stop `vita | playvalve | pgmaker | custom` \033[0m]]" << std::endl << std::endl
                << "    Query vita/play-valve/pg-maker/custom exporter." << std::endl
                << "        [[\033[32m query \033[0m]]" << std::endl << std::endl
                << "    Clear screen." << std::endl
                << "        [[\033[32m clear \033[0m]]" << std::endl << std::endl
                << "    Exit." << std::endl
                << "        [[\033[32m exit \033[0m]]" << std::endl
                << "        [[\033[32m quit \033[0m]]" << std::endl << std::endl
                << "    Trim memory." << std::endl
                << "        [[\033[32m shrink \033[0m]]" << std::endl << std::endl;
    }));
}

bool BatchMode::input() {
    return is_input;
}

void BatchMode::join(const int type) {
    if (type == 0 || type == 2) {
        vita_batch_stop_flag = true;
        if (vita_batch_thread_done && vita_batch_thread != nullptr && vita_batch_thread->joinable()) {
            std::cout << spd::BatchVitaWaitThread << std::endl;
            vita_batch_thread->join();
            std::cout << spd::BatchVitaThreadEnd << std::endl;
        } else if (!vita_batch_thread_done && vita_batch_thread != nullptr) {
            if (vita_batch_solver != nullptr)
                vita_batch_solver->stop();
            std::cout << spd::BatchVitaWaitThread << std::endl;
            vita_batch_thread->join();
            std::cout << spd::BatchVitaThreadEnd << std::endl;
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
            std::cout << spd::BatchPlayValveWaitThread << std::endl;
            playvalve_batch_thread->join();
            std::cout << spd::BatchPlayValveThreadEnd << std::endl;
        } else if (!playvalve_batch_thread_done && playvalve_batch_thread != nullptr) {
            if (playvalve_batch_solver != nullptr)
                playvalve_batch_solver->stop();
            std::cout << spd::BatchPlayValveWaitThread << std::endl;
            playvalve_batch_thread->join();
            std::cout << spd::BatchPlayValveThreadEnd << std::endl;
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
            std::cout << spd::BatchPGMakerWaitThread << std::endl;
            pgmaker_batch_thread->join();
            std::cout << spd::BatchPGMakerThreadEnd << std::endl;
        } else if (!pgmaker_batch_thread_done && pgmaker_batch_thread != nullptr) {
            if (pgmaker_batch_solver != nullptr)
                pgmaker_batch_solver->stop();
            std::cout << spd::BatchPGMakerWaitThread << std::endl;
            pgmaker_batch_thread->join();
            std::cout << spd::BatchPGMakerThreadEnd << std::endl;
            pgmaker_batch_thread.reset();
        }
        if (pgmaker_batch_solver != nullptr) {
            delete pgmaker_batch_solver;
            pgmaker_batch_solver = nullptr;
        }
        pgmaker_batch_stop_flag = false;
    }
    if (type == 0 || type == 4) {
        custom_batch_stop_flag = true;
        if (custom_batch_thread_done && custom_batch_thread != nullptr && custom_batch_thread->joinable()) {
            std::cout << spd::BatchCustomWaitThread << std::endl;
            custom_batch_thread->join();
            std::cout << spd::BatchCustomThreadEnd << std::endl;
        } else if (!custom_batch_thread_done && custom_batch_thread != nullptr) {
            if (custom_batch_solver != nullptr)
                custom_batch_solver->stop();
            std::cout << spd::BatchCustomWaitThread << std::endl;
            custom_batch_thread->join();
            std::cout << spd::BatchCustomThreadEnd << std::endl;
            custom_batch_thread.reset();
        }
        if (custom_batch_solver != nullptr) {
            delete custom_batch_solver;
            custom_batch_solver = nullptr;
        }
        custom_batch_stop_flag = false;
    }
}
