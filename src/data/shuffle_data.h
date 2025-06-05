//
// Created by baizeyv on 5/23/2025.
//

#ifndef SHUFFLE_DATA_H
#define SHUFFLE_DATA_H
#include <string>

#include "../State.h"

using namespace std;


class shuffle_data {

public:
    /**
     * current level string id
     */
    int id;
    /**
     * * 当前关卡的id string
     */
    string level;
    /**
     * * 当前关卡的序列化结果
     */
    string serialized;
    /**
     * * 当前牌局花色数量
     */
    int suit_count;

    /**
     * * constructor
     * @param id level string id
     * @param state current level root state
     */
    shuffle_data(int id, const State& state);
};


std::ostream& operator<<(std::ostream& os, const shuffle_data& data);


#endif //SHUFFLE_DATA_H
