//
// Created by baizeyv on 2025/5/1.
//

#ifndef LEVELDATA_H
#define LEVELDATA_H
#include <string>

#include "../State.h"


class LevelData {
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

    /**
     * * 第一步可以移动的步骤数量
     */
    int first_movable_count;

    /**
     * * 空出第一列需要移动的步骤数
     */
    int first_empty_column_count;

    LevelData();

    /**
     * * 用于 csv_parser
     * @param id level id
     * @param seed level seed
     * @param suit_count level suit count
     * @param calc
     * @param difficulty level difficulty
     * @param step1
     * @param step2
     * @param step3
     * @param step4
     * @param step5
     * @param step6
     * @param step7
     * @param step8
     * @param history history
     * @param level
     * @param serialized
     * @param str
     */
    LevelData(int id, const std::string &seed, int suit_count, int calc, float difficulty, int step1, int step2,
              int step3, int step4, int step5, int step6, int step7, int step8, const std::string &history,
              const std::string &level, const std::string &serialized, const std::string &str);

    LevelData(int id, const std::string &seed, int suit_count, int calc, float difficulty, int step1, int step2,
              int step3, int step4, int step5, int step6, int step7, int step8, const std::string &history,
              const std::string &level, const std::string &serialized, const std::string &str, int first_movable_count,
              int first_empty_column_count);

    LevelData(int id, const State &state, bool is_null = false);

    ~LevelData();
};

std::ostream &operator<<(std::ostream &os, const LevelData &data);


#endif // LEVELDATA_H
