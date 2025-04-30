//
// Created by baizeyv on 4/30/2025.
//

#include "Helper.h"

std::vector<std::string> Helper::split(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = 0;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }

    result.push_back(str.substr(start)); // 最后一个部分
    return result;
}
