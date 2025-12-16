//
// Created by baizeyv on 12/16/2025.
//

#ifndef SPIDERSOLVER_JOKER_H
#define SPIDERSOLVER_JOKER_H
#include <string>
#include <vector>

#include "Card.h"


/**
 * * 假 Poker/Root-State, 通过level_str生成的一个关卡,这里叫做Joker
 * ! 主要用于计算 first_movable_count 和 first_empty_column_count
 * ? 之后若要计算某个状态的某个值,可以在这个类中加,而不用使用State/Poker/Solver去增加他们的复杂度
 */
class Joker {
public:
    /**
     * * level serialized string (for example: 1,ajabkm;1,adiljh;....)
     */
    std::string serialized;

    /**
     * * deck中的50张(不同状态数量不同)
     */
    std::vector<Card> deck_cards{};

    /**
     * * 隐藏着的牌
     */
    std::vector<std::vector<Card>> hidden_cards{};

    /**
     * * 可见的牌
     */
    std::vector<std::vector<Card>> visible_cards{};

    /**
     * * constructor
     * @param level_serialized serialized
     */
    explicit Joker(const std::string &level_serialized);

    /**
     * * 获取第一步可以移动的步骤的数量
     */
    int get_first_movable_count() ;

    /**
     * * 获取第一次空出随便一列需要移动的步骤数
     * @param history 移动的历史记录
     * @return
     */
    static int get_first_empty_column_count(const std::string& history);

private:

    static std::vector<Card> find_movable_card_in_column(std::vector<Card> &column, Card* & first_card);

};


#endif // SPIDERSOLVER_JOKER_H
