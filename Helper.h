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
    static std::vector<std::string> parse_arguments(const std::string& str);
};


#endif //HELPER_H
