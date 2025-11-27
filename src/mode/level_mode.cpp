//
// Created by baizeyv on 11/20/2025.
//

#include "level_mode.h"

#include "../Solver.h"
#include "../exporter/level_exporter.h"

level_mode::level_mode() : is_input(true) {
}

level_mode::~level_mode() {
}

void level_mode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)> >;
    commands = new std::map<std::string, std::function<void()> >;

    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 3 && params.size() != 4) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        auto txt_path = params[0];
        auto output_path = params[1];
        auto txt_content = Helper::read_file(txt_path);
        auto seeds = Helper::split(txt_content, ",");
        int suit_count = 1;
        if (!Helper::try_parse_int(params[2], suit_count)) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        int max_value = 13;
        if (params.size() == 4) {
            if (!Helper::try_parse_int(params[3], max_value)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }
        const auto output = params[1] + "\\pgmaker_level\\pgmaker_" + Helper::get_current_timestamp_millis() +
                            ".csv";
        int id = 0;
        for (auto &item : seeds) {
            id ++;
            if (int seed; Helper::try_parse_int(item, seed)) {
                Solver solver(seed, suit_count, max_value, true);
                auto lv = *(solver.root_state);
                level_exporter exporter(output);
                exporter.export_csv(seed, lv);
            }
        }
    }));
    arg_commands->insert(std::make_pair("pgmakershuffle", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 3 && params.size() != 4) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        auto txt_path = params[0];
        auto output_path = params[1];
        auto txt_content = Helper::read_file(txt_path);
        auto seeds = Helper::split(txt_content, ",");
        int suit_count = 1;
        if (!Helper::try_parse_int(params[2], suit_count)) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        int max_value = 13;
        if (params.size() == 4) {
            if (!Helper::try_parse_int(params[3], max_value)) {
                std::cout << spd::PlayValveTestArgumentsException << std::endl;
                return;
            }
        }
        const auto output = params[1] + "\\pgmaker_level_shuffle\\pgmaker_" + Helper::get_current_timestamp_millis() +
                            ".csv";
        int id = 0;
        for (auto &item : seeds) {
            id ++;
            if (int seed; Helper::try_parse_int(item, seed)) {
                Solver solver(seed, suit_count, max_value, true);
                auto lv = solver.root_state->shuffle();
                level_exporter exporter(output);
                exporter.export_csv(seed, lv);
            }
        }
    }));

    commands->insert(std::make_pair("exit", [this]() {
        is_input = false;
    }));
    commands->insert(std::make_pair("quit", [this]() {
        is_input = false;
    }));
    commands->insert(std::make_pair("clear", []() {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []() {
        Helper::trim_memory();
    }));
}

bool level_mode::input() {
    return is_input;
}
