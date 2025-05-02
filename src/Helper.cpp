//
// Created by baizeyv on 4/30/2025.
//

#include "Helper.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <filesystem>

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

bool Helper::try_parse_int(const std::string& str, int& out)
{
    try
    {
        size_t pos;
        out = std::stoi(str, &pos);
        if (pos != str.length())
            return false;
        return true;
    }
    catch (...)
    {
        return false;
    }
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

std::vector<int> Helper::run_csharp_random_and_get_numbers(const std::string& exe_path, const int seed, const int count)
{
    std::vector<int> result;
    // # 构建命令,传入两个整数参数
    std::string command = "\"" + exe_path + "\" " + std::to_string(seed) + " " + std::to_string(count);
    FILE* pipe = _popen(command.c_str(), "r"); // # Windows: _popen
    if (!pipe)
    {
        std::cerr << "Failed to run c# process." << std::endl;
        return result;
    }

    std::string output;
    std::vector<char> buffer(128);
    size_t bytes_read = 0;
    while ((bytes_read = fread(buffer.data(), 1, buffer.size(), pipe)) > 0)
    {
        output.append(buffer.data(), bytes_read);
    }
    _pclose(pipe);

    try
    {
        const auto array = split(output, ",");
        for (auto& item : array)
        {
            if (int val; try_parse_int(item, val))
            {
                result.push_back(val);
            }
            else
            {
                std::cerr << "C# return value error" << std::endl;
                break;
            }
        }
    }
    catch (...)
    {
        std::cerr << "Failed to run c# process." << std::endl;
    }
    return result;
}

std::string Helper::get_current_exe_directory()
{
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    std::string full_path(path);
    const size_t last_slash = full_path.find_last_of("\\/");
    return full_path.substr(0, last_slash);
}

std::vector<int> Helper::get_randoms(const int seed, const int count)
{
    const auto exe_dir = get_current_exe_directory();
    const auto csharp_path = exe_dir + R"(\..\SpiderRandom\SpiderRandom\bin\Debug\net9.0\SpiderRandom.exe)";
    auto numbers = run_csharp_random_and_get_numbers(csharp_path, seed, count);
    return numbers;
}

void Helper::check_file_and_create_dir_when_needed(const std::string& file)
{
    std::filesystem::path p(file);
    if (!std::filesystem::exists(p.parent_path()))
    {
        std::filesystem::create_directories(p.parent_path());
    }
}

std::string Helper::read_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("Failed to open file " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string Helper::get_current_timestamp_millis()
{
    const int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return std::to_string(timestamp);
}
