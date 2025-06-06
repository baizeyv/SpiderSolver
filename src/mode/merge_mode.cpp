//
// Created by baizeyv on 6/6/2025.
//

#include "merge_mode.h"

#include "../parser/csv_parser.h"

merge_mode::merge_mode() : is_input(true) {
}

merge_mode::~merge_mode() = default;

void merge_mode::setup() {
    arg_commands = new std::map<std::string, std::function<void(const std::string &)> >;
    commands = new std::map<std::string, std::function<void()> >;

    arg_commands->insert(std::make_pair("merge", [this](const std::string &args) {
        const auto params = Helper::parse_arguments(args);
        if (params.size() < 2) {
            std::cout << spd::ViewTestArgumentsException << std::endl;
            return;
        }
        csv_parser parser;
        for (const auto& item : params) {
            parser.inject(item);
        }
        parser.filter();
        // TODO:
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

bool merge_mode::input() {
    return is_input;
}
