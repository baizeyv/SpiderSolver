//
// Created by baizeyv on 11/20/2025.
//

#include "misc_mode.h"

#include "../Joker.h"
#include "../Poker.h"
#include "../State.h"
#include "../exporter/Exporter.h"
#include "../parser/csv_parser.h"

misc_mode::misc_mode() : is_input(true) {}

misc_mode::~misc_mode() = default;

void misc_mode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)>>;
    commands = new std::map<std::string, std::function<void()>>;

    arg_commands->insert(std::make_pair("pgmaker", [](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        int seed;
        if (!Helper::try_parse_int(params[0], seed)) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        int suit_count = 1;
        if (!Helper::try_parse_int(params[1], suit_count)) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        auto pk = new Poker(seed, suit_count, 13, true);
        const auto state = new State(pk);

        // auto s = state->shuffle();

        std::cout << std::endl << state->to_level_str() << std::endl;

        delete state;
        delete pk;
    }));
    arg_commands->insert(std::make_pair("pgmakerspec", [](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        int suit_count = 1;
        if (!Helper::try_parse_int(params[1], suit_count)) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        const auto lv_str = params[0];
        const auto array = Helper::split(lv_str, ",");
        std::vector<int> cds;
        for (const auto &item: array) {
            cds.push_back(std::stoi(item));
        }
        auto pk = new Poker(cds, suit_count);
        const auto state = new State(pk);

        // auto s = state->shuffle();

        std::cout << std::endl << state->to_level_str() << std::endl;

        delete state;
        delete pk;
    }));
    arg_commands->insert(std::make_pair("pgmakerspecshuffle", [](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 2) {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        int suit_count = 1;
        if (!Helper::try_parse_int(params[1], suit_count)) {
            std::cout << spd::PlayValveTestArgumentsException << std::endl;
            return;
        }
        const auto lv_str = params[0];
        const auto array = Helper::split(lv_str, ",");
        std::vector<int> cds;
        for (const auto &item: array) {
            cds.push_back(std::stoi(item));
        }
        auto pk = new Poker(cds, suit_count);
        const auto state = new State(pk);

        const auto s = state->shuffle();

        std::cout << std::endl << s.to_level_str() << std::endl;

        delete state;
        delete pk;
    }));

    // # 迪迦
    arg_commands->insert(std::make_pair("tiga", [](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() != 1) {
            std::cout << spd::VitaTestArgumentsException << std::endl;
            return;
        }
        const auto& input_file = params[0];
        const auto output_file = input_file.substr(0, input_file.find_last_of('.')) + "_tiga.csv";
        bool skip_flag = true;
        for (const auto data = csv_parser::parse(input_file); const auto &item: data) {
            if (skip_flag) {
                skip_flag = false;
                continue;
            }
            int id;
            Helper::try_parse_int(item.id, id);
            int suit_count;
            Helper::try_parse_int(item.suit_count, suit_count);
            int calc;
            Helper::try_parse_int(item.calc, calc);
            float difficulty;
            Helper::try_parse_float(item.difficulty, difficulty);
            int step1;
            Helper::try_parse_int(item.step1, step1);
            int step2;
            Helper::try_parse_int(item.step2, step2);
            int step3;
            Helper::try_parse_int(item.step3, step3);
            int step4;
            Helper::try_parse_int(item.step4, step4);
            int step5;
            Helper::try_parse_int(item.step5, step5);
            int step6;
            Helper::try_parse_int(item.step6, step6);
            int step7;
            Helper::try_parse_int(item.step7, step7);
            int step8;
            Helper::try_parse_int(item.step8, step8);

            Joker joker(item.serialized);

            const auto ld =
                    LevelData(id, item.seed, suit_count, calc, difficulty, step1, step2, step3, step4, step5, step6,
                              step7, step8, item.history, item.level, item.serialized, item.str,
                              joker.get_first_movable_count(), joker.get_first_empty_column_count_by_state(item.history));

            const auto e = Exporter(output_file);
            e.export_csv(ld);
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

bool misc_mode::input() { return is_input; }
