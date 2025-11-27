//
// Created by baizeyv on 4/30/2025.
//

#include "Helper.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <psapi.h>
#include <filesystem>
#include <random>

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

void Helper::ltrim(std::string &s) {
    if (s.empty())
        return;
    s.erase(s.begin(), std::ranges::find_if(s, [](const unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void Helper::rtrim(std::string &s) {
    if (s.empty())
        return;
    const auto it = std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    });
    s.erase(it.base(), s.end());
}

void Helper::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

bool Helper::try_parse_int(const std::string &str, int &out) {
    try {
        size_t pos;
        out = std::stoi(str, &pos);
        if (pos != str.length())
            return false;
        return true;
    } catch (...) {
        return false;
    }
}

bool Helper::try_parse_float(const std::string &str, float &out) {
    try {
        size_t pos;
        out = std::stof(str, &pos);
        if (pos != str.length())
            return false;
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<std::string> Helper::parse_arguments(const std::string &str) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    while (true) {
        std::string arg;
        stream >> arg;
        trim(arg);
        if (arg.empty())
            break;
        result.push_back(arg);
    }
    return result;
}

std::vector<int>
Helper::run_csharp_random_and_get_numbers(const std::string &exe_path, const int seed, const int count) {
    std::vector<int> result;
    // # 构建命令,传入两个整数参数
    std::string command = "\"" + exe_path + "\" " + std::to_string(seed) + " " + std::to_string(count);
    FILE *pipe = _popen(command.c_str(), "r"); // # Windows: _popen
    if (!pipe) {
        std::cerr << "Failed to run c# process." << std::endl;
        return result;
    }

    std::string output;
    std::vector<char> buffer(128);
    size_t bytes_read = 0;
    while ((bytes_read = fread(buffer.data(), 1, buffer.size(), pipe)) > 0) {
        output.append(buffer.data(), bytes_read);
    }
    _pclose(pipe);

    try {
        const auto array = split(output, ",");
        for (auto &item: array) {
            if (int val; try_parse_int(item, val)) {
                result.push_back(val);
            } else {
                std::cerr << "C# return value error" << std::endl;
                break;
            }
        }
    } catch (...) {
        std::cerr << "Failed to run c# process." << std::endl;
    }
    return result;
}

std::string Helper::get_current_exe_directory() {
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    std::string full_path(path);
    const size_t last_slash = full_path.find_last_of("\\/");
    return full_path.substr(0, last_slash);
}

std::vector<int> Helper::get_randoms(const int seed, const int count) {
    prepare_spider_random_exe();
    std::string csharp_path;
    if (SpiderRandom.empty()) {
        const auto exe_dir = get_current_exe_directory();
        csharp_path = exe_dir + R"(\..\SpiderRandom\SpiderRandom\bin\Debug\net9.0\SpiderRandom.exe)";
    } else {
        csharp_path = SpiderRandom;
    }
    auto numbers = run_csharp_random_and_get_numbers(csharp_path, seed, count);
    return numbers;
}

void Helper::check_file_and_create_dir_when_needed(const std::string &file) {
    std::filesystem::path p(file);
    if (!std::filesystem::exists(p.parent_path())) {
        std::filesystem::create_directories(p.parent_path());
    }
}

std::string Helper::read_file(const std::string &path) {
    const std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> Helper::read_file_line(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file " + path);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

std::string Helper::get_current_timestamp_millis() {
    const int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return std::to_string(timestamp);
}

void Helper::prepare_spider_random_exe() {
    if (!SpiderRandom.empty() && std::filesystem::exists(SpiderRandom))
        return;
    HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(101), RT_RCDATA);
    if (hRes) {
        HGLOBAL hData = LoadResource(NULL, hRes);
        DWORD size = SizeofResource(NULL, hRes);
        void *pData = LockResource(hData);

        std::string tempPath = getenv("TEMP");
        std::string exePath = tempPath + "\\SpiderRandom.exe";
        std::ofstream out(exePath, std::ios::binary);
        out.write(reinterpret_cast<char *>(pData), size);
        out.close();
        SpiderRandom = exePath;
        /*
        auto pipe = _popen(exePath.c_str(), "r");
        if (!pipe)
        {
            std::cerr << "Failed to run c# process." << std::endl;
        } else
        {
            std::string output;
            std::vector<char> buffer(128);
            size_t bytes_read = 0;
            while ((bytes_read = fread(buffer.data(), 1, buffer.size(), pipe)) > 0)
            {
                output.append(buffer.data(), bytes_read);
            }
            _pclose(pipe);
        }
        */
    }
}

std::vector<int> Helper::shuffle_vector_differentFromOriginal(const std::vector<int> &input) {
    std::vector<int> result = input;
    std::random_device rd;
    std::mt19937 g(rd());
    do {
        std::ranges::shuffle(result, g);
    } while (result == input);
    return result;
}

size_t Helper::get_memory_usage() {
    // PROCESS_MEMORY_COUNTERS pmc;
    // const HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
    //
    // if (hProcess == NULL) {
    //     return 0;
    // }
    //
    // if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
    //     CloseHandle(hProcess);
    //     return pmc.WorkingSetSize / 1024;  // 返回 KB
    // }
    //
    // CloseHandle(hProcess);
    // return 0;
    PROCESS_MEMORY_COUNTERS_EX pmc;
    HANDLE hProcess = GetCurrentProcess();

    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *) &pmc, sizeof(pmc))) {
        // std::cout << "PrivateUsage (真实占用): " << pmc.PrivateUsage / 1024 << " KB\n";
        return pmc.PrivateUsage / 1024;
    } else {
        // std::cerr << "GetProcessMemoryInfo failed.\n";
        return 0;
    }
}

void Helper::trim_memory() {
    // -1 表示让系统自动决定新的最小/最大工作集，效果是释放尽量多的物理内存
    SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
}
