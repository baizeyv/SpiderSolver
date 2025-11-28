//
// Created by baizeyv on 5/23/2025.
//

#include "shuffle_mode.h"
#include "../Solver.h"
#include "../data/VitaBean.h"
#include "../data/VitaBean2.h"
#include "../exporter/shuffle_exporter.h"
#include "../json.hpp"

shuffle_mode::shuffle_mode() : is_input(true) {}

shuffle_mode::~shuffle_mode() = default;

void shuffle_mode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("vita", [](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        const auto &json_path = params[0];
        auto json_content = Helper::read_file(json_path);
        const nlohmann::json j = nlohmann::json::parse(json_content);

        const auto output = params[1] + "\\vita_shuffle\\vita_" + Helper::get_current_timestamp_millis() + ".csv";
        try {
            const auto vita_bean = j.get<VitaBean>();
            for (const auto &item: vita_bean.data) {
                for (const auto &level: item.second) {
                    Solver solver(level.question);
                    auto shuffled = solver.root_state->shuffle();
                    shuffle_exporter exporter(output);
                    if (int id; Helper::try_parse_int(level.id, id)) {
                        exporter.export_csv(id, shuffled);
                    }
                }
            }
        } catch (...) {
            auto vita_bean = j.get<VitaBean2>();
            for (const auto &item: vita_bean.data) {
                for (const auto &level: item.second) {
                    Solver solver(level.question);
                    auto shuffled = solver.root_state->shuffle();
                    shuffle_exporter exporter(output);
                    exporter.export_csv(level.id, shuffled);
                }
            }
        }
    }));
    arg_commands->insert(std::make_pair("playvalve", [](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 3 && params.size() != 4) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        const auto &txt_path = params[0];
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
        const auto output =
                params[1] + "\\playvalve_shuffle\\playvalve_" + Helper::get_current_timestamp_millis() + ".csv";
        int id = 0;
        for (auto &item: seeds) {
            id++;
            if (int seed; Helper::try_parse_int(item, seed)) {
                Solver solver(seed, suit_count, max_value);
                auto shuffled = solver.root_state->shuffle();
                shuffle_exporter exporter(output);
                exporter.export_csv(seed, shuffled);
            }
        }
    }));
    commands->insert(std::make_pair("exit", [this]() { is_input = false; }));
    commands->insert(std::make_pair("quit", [this]() { is_input = false; }));
    commands->insert(std::make_pair("clear", []() {
        system("cls");
        spd::output_icon();
    }));
    commands->insert(std::make_pair("shrink", []() { Helper::trim_memory(); }));
}

bool shuffle_mode::input() { return is_input; }
