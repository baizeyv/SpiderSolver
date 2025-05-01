//
// Created by baizeyv on 4/29/2025.
//

#ifndef CARD_H
#define CARD_H
#include <string>


class Card {
public:
    /**
     * * 牌面值
     */
    int value;

    /**
     * * 牌面花色
     */
    int suit;

    int original_value;

    explicit Card(int value, const int suit);

    std::string get_suit() const;

    std::string get_value() const;

    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& out, const Card& card);

};

bool operator==(const Card& lhs, const Card& rhs);

std::ostream& operator<<(std::ostream& out, const Card& card);

std::ostream& operator<<(std::ostream& out, const Card*& card);

#endif //CARD_H
