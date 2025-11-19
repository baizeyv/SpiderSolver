//
// Created by baizeyv on 4/29/2025.
//

#ifndef POKER_H
#define POKER_H
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "Card.h"


class Poker {
public:
    /**
     * * 牌局标识符 (play valve -> seed || vita -> level string)
     */
    std::string mark;

    /**
     * * 花色数量
     */
    int suitCount;

    int max_value;

    /**
     * * playvalve 和 pgmaker 的 deck 输出需要翻转
     */
    bool reverse_output;

    Poker(int seed, int suit_count, int max_value = 13, bool pg_maker = false);

    Poker(const std::string& asVitaLevel);

    std::string get_string() const;

    friend std::ostream& operator<<(std::ostream& out, const Poker& poker);

private:

    std::unordered_map<char, int> vita_char_map;

    static std::vector<Card> generate_deck(int seed, int suitCount, int max_value = 13, bool pg_maker = false);

    // static std::vector<Card> vita_level_convert_to_poker(const std::string &vitaLevel, const Poker* poker);
    //
    // static int vita_char_to_card_value(char c, Poker* &poker);

    static char card_value_to_char(int x);

    static Card build_card(int suit, int value);


public:
    /**
     * * 牌堆
     */
    std::vector<Card> cards;

    std::string get_level() const;

    std::string to_serialized() const;

};



#endif //POKER_H
