//
// Created by baizeyv on 5/12/2025.
//

#include "csv_parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "../Helper.h"

void csv_parser::inject(const std::string &filename) {
    const std::vector<csv_data> data_vec = parse(filename);
    const auto vec = convert(data_vec);
    for (const auto &data : vec) {
        if (!data_map.contains(data.seed)) {
            std::vector<LevelData> tmp;
            tmp.push_back(data);
            data_map[data.seed] = tmp;
        } else {
            data_map[data.seed].push_back(data);
        }
    }
}

void csv_parser::filter() {
    std::vector<LevelData> storage;
    for (const auto& [key, value] : data_map) {
        if (value.empty())
            continue;
        // # 是否全部是无效数据
        bool all_rubbish = true;
        LevelData min;
        min.step8 = 200000000;
        for (const auto& data : value) {
            if (data.step8 > 0) {
                all_rubbish = false;
                if (data.step8 < min.step8) {
                    min = data;
                } else if (data.step8 == min.step8) {
                    if (data.calc < min.calc) {
                        min = data;
                    }
                }
            }
        }
        if (all_rubbish) {
            storage.push_back(value.front());
        } else {
            storage.push_back(min);
        }
    }
    const auto exe_dir = Helper::get_current_exe_directory();
    const auto output = exe_dir + "\\merged\\merged_" + Helper::get_current_timestamp_millis() + ".csv";

    // # 写入新文件
    Helper::check_file_and_create_dir_when_needed(output);
    if (std::filesystem::exists(output)) {
        // # 文件存在则先删除
        std::filesystem::remove(output);
    }
    std::ofstream writer(output, std::ios::app);
    writer << "id,seed,calc,difficulty,step1,step2,step3,step4,step5,step6,step7,step8,suitCount,history,level,serialized\n";
    for (auto& item : storage) {
        std::ostringstream oss;
        oss << item;
        auto content = oss.str();
        writer << content << "\n";
    }
}

std::vector<csv_data> csv_parser::parse(const std::string &filename) {
    std::vector<csv_data> result;
    if (!std::filesystem::exists(filename)) {
        std::cout << "File does not exist." << std::endl;
        // # 目标文件不存在
        return result;
    }
    std::ifstream file(filename);

    // # 准备逐行读取
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) // # 空行
            break;
        csv_data data = parse(line);
        result.push_back(data);
    }
    return result;
}

csv_data csv_parser::parse(std::string &line) {
    // # 是否在引号中
    bool in_quotes = false;
    std::string current;
    std::vector<std::string> result;

    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (in_quotes) {
            if (c == '"') {
                // # 看下一个字符是否也是"
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    current += '"';
                    ++i;
                } else {
                    in_quotes = false; // # 引号闭合
                }
            } else {
                current += c;
            }
        } else {
            if (c == '"') {
                in_quotes = true; // # 开始引号
            } else if (c == ',') {
                result.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }
    }
    result.push_back(current);
    csv_data data;
    data.id = result[0];
    data.seed = result[1];
    data.calc = result[2];
    data.difficulty = result[3];
    data.step1 = result[4];
    data.step2 = result[5];
    data.step3 = result[6];
    data.step4 = result[7];
    data.step5 = result[8];
    data.step6 = result[9];
    data.step7 = result[10];
    data.step8 = result[11];
    data.suit_count = result[12];
    data.history = result[13];
    data.level = result[14];
    data.serialized = result[15];
    data.str = result[16];
    return data;
}

LevelData csv_parser::parse(const csv_data &data) {
    int id;
    if (!Helper::try_parse_int(data.id, id)) {
        id = 0;
    }
    int suit_count;
    if (!Helper::try_parse_int(data.suit_count, suit_count)) {
        std::cout << "Invalid suit count." << std::endl;
        return LevelData();
    }
    int calc;
    if (!Helper::try_parse_int(data.calc, calc)) {
        std::cout << "Invalid calculation." << std::endl;
        return LevelData();
    }
    float difficulty;
    if (!Helper::try_parse_float(data.difficulty, difficulty)) {
        std::cout << "Invalid difficulty." << std::endl;
        return LevelData();
    }
    int step1;
    if (!Helper::try_parse_int(data.step1, step1)) {
        std::cout << "Invalid step1." << std::endl;
        return LevelData();
    }
    int step2;
    if (!Helper::try_parse_int(data.step2, step2)) {
        std::cout << "Invalid step2." << std::endl;
        return LevelData();
    }
    int step3;
    if (!Helper::try_parse_int(data.step3, step3)) {
        std::cout << "Invalid step3." << std::endl;
        return LevelData();
    }
    int step4;
    if (!Helper::try_parse_int(data.step4, step4)) {
        std::cout << "Invalid step4." << std::endl;
        return LevelData();
    }
    int step5;
    if (!Helper::try_parse_int(data.step5, step5)) {
        std::cout << "Invalid step5." << std::endl;
        return LevelData();
    }
    int step6;
    if (!Helper::try_parse_int(data.step6, step6)) {
        std::cout << "Invalid step6." << std::endl;
        return LevelData();
    }
    int step7;
    if (!Helper::try_parse_int(data.step7, step7)) {
        std::cout << "Invalid step7." << std::endl;
        return LevelData();
    }
    int step8;
    if (!Helper::try_parse_int(data.step8, step8)) {
        std::cout << "Invalid step8." << std::endl;
        return LevelData();
    }

    return LevelData(id, data.seed, suit_count, calc, difficulty, step1, step2, step3, step4, step5, step6, step7,
                     step8, data.history, data.level, data.serialized, data.str);
}

std::vector<LevelData> csv_parser::convert(const std::vector<csv_data> &data) {
    std::vector<LevelData> vec;
    for (auto &item: data) {
        vec.push_back(parse(item));
    }
    return vec;
}
