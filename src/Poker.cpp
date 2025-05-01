//
// Created by baizeyv on 4/29/2025.
//

#include "Poker.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>
#include <unordered_set>

#include "Helper.h"

/**
 * * Public Constructor
 * @param seed random seed
 * @param suit_count suit count
 */
Poker::Poker(const int seed, const int suit_count) : suitCount(suit_count) {
    mark = std::to_string(seed);
    cards = generate_deck(seed, suit_count);
}

Poker::Poker(const std::string& asVitaLevel) {
    mark = asVitaLevel;

    const auto array = Helper::split(asVitaLevel, ",1;");
    // # 牌堆
    auto deck = array.back().substr(0, array.back().length() - 2);

    std::string value;
    int idx = 0;
    // # 遍历10列
    for (int x = 0; x < 6; x ++) {
        for (int i = 0; i < 10; i ++) {
            const auto& str = array[i];
            if (str.length() <= x)
                continue;
            auto c = str[x];
            value += c;
        }
        idx++;
    }

    auto s = value + deck;
    auto get_card = [](const int x) {
        if (x >= 1 && x <= 13) {
            return build_card(2, x);
        }
        if (x >= 14 && x <= 26) {
            return build_card(1, x);
        }
        if (x >= 27 && x <= 39) {
            return build_card(4, x);
        }
        if (x >= 40 && x <= 53) {
            return build_card(3, x);
        }
        return build_card(999, 999);
    };
    for (char& item : s) {
        if (vita_char_map.empty()) {
            for (int i = 1; i <= 54; i ++) {
                char val = card_value_to_char(i);
                vita_char_map.insert({val, i});
            }
        }
        int v = vita_char_map.at(item);
        cards.push_back(get_card(v));
    }
    // ########################################################
    std::unordered_set<int> values;
    for (auto & card : cards) {
        values.insert(card.original_value);
    }
    if (values.size() == 13)
        suitCount = 1;
    else if (values.size() == 26)
        suitCount = 2;
    else if (values.size() == 39)
        suitCount = 3;
    else if (values.size() == 52)
        suitCount = 4;
    else
        suitCount = -1;
}

std::string Poker::get_string() const {
    std::string res;
    for (size_t i = 0; i < 44; i ++) {
        if (i % 10 == 0)
            res += "\n";
        res += cards[i].to_string();
    }
    res += "\n";
    res += "\n";
    for (size_t i = 50; i < 54; i ++) {
        res += cards[i].to_string();
    }
    for (size_t i = 44; i < 50; i ++) {
        res += cards[i].to_string();
    }
    res += "\n";
    res += "\n";
    int idx = 0;
    for (size_t i = cards.size() - 1; i >= 54; i --) {
        if (idx != 0 && idx % 10 == 0)
            res += "\n";
        res += cards[i].to_string();
        idx ++;
    }
    return res;
}

std::vector<Card> Poker::generate_deck(const int seed, const int suitCount) {
    std::vector<int> cards;
    for (int i = 0; i < 13; i ++) {
        for (int j = 1; j <= 8; j ++) {
            const int tmp = j % suitCount;
            cards.push_back(i + 1 + tmp * 13);
        }
    }

    auto rds = Helper::get_randoms(seed, 104);
    std::vector<std::pair<int, int>> key_vec;
    for (size_t i = 0; i < cards.size(); i ++)
    {
        auto random_value = rds[i];
        key_vec.emplace_back(random_value, cards[i]);
    }
    std::ranges::stable_sort(key_vec);
    for (size_t i = 0; i < cards.size(); i ++)
    {
        cards[i] = key_vec[i].second;
    }

    
    std::vector<Card> deck;
    for (const int card : cards) {
        if (card >= 1 && card <= 13) {
            deck.push_back(build_card(2, card));
        } else if (card >= 14 && card <= 26) {
            deck.push_back(build_card(1, card));
        } else if (card >= 27 && card <= 39) {
            deck.push_back(build_card(4, card));
        } else if (card >= 40 && card <= 52) {
            deck.push_back(build_card(3, card));
        } else {
            deck.push_back(build_card(-1, card));
        }
    }
    return deck;
}

/*
std::vector<Card> Poker::vita_level_convert_to_poker(const std::string &vitaLevel, const Poker* poker) {
    auto array = Helper::split(vitaLevel, ",1;");
    std::string deck;
    // # 牌堆
    auto deckString = array.back().substr(0, array.back().length() - 2);
    for (size_t i = deckString.length() - 1 ; i >= 0; i --) {
        deck += deckString[i];
    }

    std::string value = "";
    int idx = 0;
    // # 遍历10列
    for (int x = 0; x < 6; x ++) {
        for (int i = 0; i < 10; i ++) {
            const auto& str = array[i];
            if (str.length() <= x)
                continue;
            auto c = str[x];
            value += c;
        }
        idx++;
    }

    auto s = value + deck;
    auto get_card = [](const int x) {
        if (x >= 1 && x <= 13) {
            return build_card(2, x);
        }
        if (x >= 14 && x <= 26) {
            return build_card(1, x);
        }
        if (x >= 27 && x <= 39) {
            return build_card(4, x);
        }
        if (x >= 40 && x <= 53) {
            return build_card(3, x);
        }
        return build_card(999, 999);
    };
    std::vector<Card> result;
    for (char& item : s) {
        // int v = vita_char_to_card_value(item, poker);
        // result.push_back(get_card(v));
    }
    return result;
}

int Poker::vita_char_to_card_value(const char c, Poker *&poker) {
    if (poker->vita_char_map.empty()) {
        for (int i = 1; i <= 54; i ++) {
            char val = card_value_to_char(i);
            poker->vita_char_map.insert({val, i});
        }
    }
    return poker->vita_char_map.at(c);
}
*/

char Poker::card_value_to_char(const int x) {
    if (x < 14)
        return ('N' + (x - 1));
    if (x < 27)
        return ('n' + (x - 14));
    if (x < 40)
        return ('a' + (x - 27));
    return 'A' + (x - 40);
}

Card Poker::build_card(const int suit, const int value) {
    if (suit >= 1 && suit <= 4) {
        // # 有效花色
        return Card(value, suit);
    }
    // # 无效花色
    throw std::string("Error On `build_card` -> Invalid Suit");
    // return Card(-1, -1);
}

std::ostream & operator<<(std::ostream &out, const Poker &poker) {
    out << poker.get_string();
    return out;
}
