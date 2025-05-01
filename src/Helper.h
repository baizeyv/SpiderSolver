//
// Created by baizeyv on 4/30/2025.
//

#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <vector>


class Helper
{
public:
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    static void ltrim(std::string& s);
    static void rtrim(std::string& s);
    static void trim(std::string& s);
    static bool try_parse_int(const std::string& str, int& out);
    static std::vector<std::string> parse_arguments(const std::string& str);
    static std::vector<int> run_csharp_random_and_get_numbers(const std::string& exe_path, int seed, int count);
    static std::string get_current_exe_directory();
    static std::vector<int> get_randoms(int seed, int count);
    static void check_file_and_create_dir_when_needed(const std::string& file);
};


#endif //HELPER_H
