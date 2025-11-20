//
// Created by baizeyv on 2025/5/1.
//

#ifndef LEVELDATA_H
#define LEVELDATA_H
#include <string>

#include "../State.h"


class LevelData
{
public:
    int id;
    std::string seed;
    int suit_count;
    int calc;
    float difficulty;
    int step1;
    int step2;
    int step3;
    int step4;
    int step5;
    int step6;
    int step7;
    int step8;
    std::string history;
    std::string level;
    std::string serialized;
    std::string str;

    LevelData();
    LevelData(int id, const std::string &seed, int suit_count, int calc, float difficulty, int step1, int step2, int step3, int step4, int step5, int step6, int step7, int step8, const std::string &history, const std::string &level, const std::string &serialized, const std::string & str);
    LevelData(int id, const State& state, bool is_null = false);
    ~LevelData();
};

std::ostream& operator<<(std::ostream& os, const LevelData& data);


#endif //LEVELDATA_H
