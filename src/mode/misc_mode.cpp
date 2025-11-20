//
// Created by baizeyv on 11/20/2025.
//

#include "misc_mode.h"

#include "../Poker.h"
#include "../State.h"

misc_mode::misc_mode() : is_input(true) {
}

misc_mode::~misc_mode() {
}

void misc_mode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)> >;
    commands = new std::map<std::string, std::function<void()> >;

    arg_commands->insert(std::make_pair("pgmaker", [this](const std::string &args) {
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

    arg_commands->insert(std::make_pair("pgmakerspec", [this](const std::string &args) {
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
        for (const auto& item : array) {
            cds.push_back(std::stoi(item));
        }
        auto pk = new Poker(cds, suit_count);
        const auto state = new State(pk);

        // auto s = state->shuffle();

        std::cout << std::endl << state->to_level_str() << std::endl;

        delete state;
        delete pk;
    }));


    arg_commands->insert(std::make_pair("pgmakerspecshuffle", [this](const std::string &args) {
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
        for (const auto& item : array) {
            cds.push_back(std::stoi(item));
        }
        auto pk = new Poker(cds, suit_count);
        const auto state = new State(pk);

        const auto s = state->shuffle();

        std::cout << std::endl << s.to_level_str() << std::endl;

        delete state;
        delete pk;
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

bool misc_mode::input() {
    return is_input;
}
