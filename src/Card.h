//
// Created by baizeyv on 4/29/2025.
//

#ifndef CARD_H
#define CARD_H
#include <cstdint>
#include <string>


class Card {
public:
    /**
     * * 牌面值
     */
    uint8_t value : 4;

    /**
     * * 牌面花色
     */
    uint8_t suit : 2;

    uint8_t original_value : 6;

    explicit Card(int value, const int suit);

    std::string get_suit() const;

    std::string get_value() const;

    std::string to_string() const;

    char to_char() const;

    static int to_value(char c);

    friend std::ostream& operator<<(std::ostream& out, const Card& card);

};

bool operator==(const Card& lhs, const Card& rhs);

std::ostream& operator<<(std::ostream& out, const Card& card);

std::ostream& operator<<(std::ostream& out, const Card*& card);

#endif //CARD_H
