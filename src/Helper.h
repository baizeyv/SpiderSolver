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
    static std::string read_file(const std::string& path);
    static std::string get_current_timestamp_millis();
    static void prepare_spider_random_exe();
    static size_t get_memory_usage();
    static void trim_memory();
};
inline std::string SpiderRandom;

template <typename T>
size_t get_vector_memory(const std::vector<T>& vec)
{
    return sizeof(T) * vec.capacity();
}

template <typename T>
size_t get_nested_vector_memory(const std::vector<std::vector<T>>& nestedVec)
{
    size_t total = get_vector_memory(nestedVec);
    for (const auto& inner : nestedVec)
    {
        total += get_vector_memory(inner);
    }
    return total;
}


#endif //HELPER_H
