//
// Created by baizeyv on 4/29/2025.
//

#include "Card.h"

#include <string>

bool operator==(const Card &lhs, const Card &rhs) {
    return lhs.suit == rhs.suit && lhs.value == rhs.value;
}

std::ostream& operator<<(std::ostream &out, const Card &card) {
    out << card.get_suit() << card.get_value();
    return out;
}

Card::Card(int value, const int suit) : suit(suit - 1) {
    original_value = value;
    if (value <= 52 && value >= 40)
        value -= 39;
    else if (value  <= 39 &&value >= 27)
        value -= 26;
    else if (value <= 26 && value >= 14)
        value -= 13;
    this->value = value;
}

std::string Card::get_suit() const {
    switch (suit) {
        case 0:
            return "S"; // Spades 黑桃
        case 1:
            return "H"; // Heart 红桃
        case 2:
            return "D"; // Diamonds 方片
        case 3:
            return "C"; // Clubs 梅花
        default:
            throw std::string("Error On `GetSuit()` -> Undefined suid.");
    }
}

std::string Card::get_value() const {
    if (value == 10)
        return "[X] ";
    if (value == 11)
        return "[J] ";
    if (value == 12)
        return "[Q] ";
    if (value == 13)
        return "[K] ";
    return "[" + std::to_string(value) + "] ";
}

std::string Card::to_string() const {
    return get_suit() + get_value();
}

char Card::to_char() const
{
    if (const auto offest_index = original_value - 1; offest_index < 26)
        return 'a' + offest_index;
    else
        return 'A' + offest_index - 26;
}

int Card::to_value(const char c) {
    // * Z->52 A->27
    // * z->26 a->1
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 1;
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 27;
    }
    return 999;
}
