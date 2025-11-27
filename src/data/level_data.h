//
// Created by baizeyv on 11/20/2025.
//

#ifndef SPIDERSOLVER_LEVEL_DATA_H_S
#define SPIDERSOLVER_LEVEL_DATA_H_S

#include <string>

#include "../State.h"

using namespace std;

class level_data {
public:
    /**
    * * current level string id
    */
    int id;
    /**
     * * 当前牌局花色数量
     */
    int suit_count;

    /**
     * * 数字记录关卡
     */
    string level_num;

    /**
     * * 字符记录关卡
     */
    string level_str;

    level_data(int id, const State& state);
};

std::ostream& operator<<(std::ostream& os, const level_data& data);

#endif //SPIDERSOLVER_LEVEL_DATA_H_S
