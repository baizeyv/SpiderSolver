//
// Created by baizeyv on 4/30/2025.
//

#include "Helper.h"

#include <algorithm>
#include <sstream>

std::vector<std::string> Helper::split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = 0;

    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }

    result.push_back(str.substr(start)); // 最后一个部分
    return result;
}

void Helper::ltrim(std::string& s)
{
    if (s.empty())
        return;
    s.erase(s.begin(), std::ranges::find_if(s, [](const unsigned char ch)
    {
        return !std::isspace(ch);
    }));
}

void Helper::rtrim(std::string& s)
{
    if (s.empty())
        return;
    const auto it = std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    });
    s.erase(it.base(), s.end());
}

void Helper::trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

std::vector<std::string> Helper::parse_arguments(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream stream(str);
    while (true)
    {
        std::string arg;
        stream >> arg;
        trim(arg);
        if (arg.empty())
            break;
        result.push_back(arg);
    }
    return result;
}
