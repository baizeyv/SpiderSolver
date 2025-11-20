//
// Created by baizeyv on 4/29/2025.
//

#include "Poker.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>

#include "Helper.h"
#include "State.h"

/**
 * * Public Constructor
 * @param seed random seed
 * @param suit_count suit count
 * @param max_value
 * @param pg_maker 是否是pgMaker的
 */
Poker::Poker(const int seed, const int suit_count, const int max_value, const bool pg_maker) : suitCount(suit_count), max_value(max_value), reverse_output(true) {
    mark = std::to_string(seed);
    cards = generate_deck(seed, suit_count, max_value, pg_maker);
}

Poker::Poker(const std::string& asVitaLevel) : max_value(13), reverse_output(false) {
    mark = asVitaLevel;

    const auto array = Helper::split(asVitaLevel, ",1;");
    // # 牌堆
    const auto deck = array.back().substr(0, array.back().length() - 2);

    std::string value;
    int idx = 0;
    // # 遍历10列
    for (int x = 0; x < 6; x ++) {
        for (int i = 0; i < 10; i ++) {
            const auto& str = array[i];
            if (str.length() <= x)
                continue;
            const auto c = str[x];
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
        const int v = vita_char_map.at(item);
        cards.push_back(get_card(v));
    }
    // ########################################################
    std::unordered_set<int> values;
    for (const auto & card : cards) {
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
    if (reverse_output) {
        for (size_t i = 54; i < cards.size(); i ++) {
            if (idx != 0 && idx % 10 == 0)
                res += "\n";
            res += cards[i].to_string();
            idx ++;
        }
    } else {
        for (size_t i = cards.size() - 1; i >= 54; i --) {
            if (idx != 0 && idx % 10 == 0)
                res += "\n";
            res += cards[i].to_string();
            idx ++;
        }
    }
    return res;
}

std::vector<Card> Poker::generate_deck(const int seed, const int suitCount, const int max_value, const bool pg_maker) {
    std::vector<int> cards;

    if (pg_maker) {
        // # pgmaker
        for (int j = 0; j < 8; j ++) {
            for (int i = 0; i < 13; i ++) {
                cards.push_back(j * 13 + i);
            }
        }
    } else {
        // # playvalve
        for (int i = 0; i < max_value; i ++) {
            for (int j = 1; j <= 8; j ++) {
                const int tmp = j % suitCount;
                cards.push_back(i + 1 + tmp * 13);
            }
        }
    }

    const auto rds = Helper::get_randoms(seed, 104);
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

    if (pg_maker) {
        std::vector<int> tmpNew;
        for (int k = 0; k < 2; k ++) {
            for (int j = 0; j < 4; j ++) {
                auto tmp = j % suitCount;
                for (int i = 0; i < 13; i ++) {
                    tmpNew.push_back(tmp * 13 + i + 1);
                }
            }
        }

        std::vector<int> question;
        for (size_t i = 0; i < tmpNew.size(); i ++) {
            question.push_back(tmpNew[cards[i]]);
        }

        std::vector<int> res;
        for (size_t i = 50; i < 94; i ++) { // # 所有隐藏牌
            res.push_back(question[i]);
        }
        for (size_t i = 98; i < 104; i ++) { // # 右侧6个可见的
            res.push_back(question[i]);
        }
        for (size_t i = 94; i < 98; i ++) { // # 左侧4个可见
            res.push_back(question[i]);
        }
        for (size_t i = 0; i < 50; i ++) {
            res.push_back(question[i]);
        }


        // # 将question赋值给cards
        for (size_t i = 0; i < cards.size(); i ++)
        {
            cards[i] = res[i];
        }
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

std::string Poker::get_level() const
{
    std::string result;
    for (size_t i = 0; i < cards.size(); i ++)
    {
        result += std::to_string(cards[i].original_value);
        if (i != cards.size() - 1)
        {
            result += ",";
        }
    }
    return result;
}

std::string Poker::to_serialized() const
{
    std::vector<std::vector<Card> > hiddenCards;
    std::vector<std::vector<Card> > visibleCards;
    std::vector<Card> deckCard;

    // # 44张隐藏的
    for (int i = 0; i < 44; ++i) {
        const int tmp = i % 10;
        if (hiddenCards.size() <= tmp) {
            hiddenCards.resize(tmp + 1);
        }
        hiddenCards[tmp].push_back(cards[i]);
    }
    for (int i = 44; i < 54; i++) {
        const int tmp = i % 10;
        if (visibleCards.size() <= tmp) {
            visibleCards.resize(tmp + 1);
        }
        visibleCards[tmp].push_back(cards[i]);
    }
    // # 初始化牌堆指针数组
    for (int i = 54; i < cards.size(); i++) {
        deckCard.push_back(cards[i]);
    }
    for (auto& item : hiddenCards) {
        std::ranges::reverse(item);
    }
    if (!reverse_output) {
        // # playvalve 和 pgmaker 的在这里不需要翻转
        std::ranges::reverse(deckCard);
    }

    
    std::string result;
    for (size_t i = 0; i < 10; i ++)
    {
        result += std::to_string(visibleCards[i].size());
        result += ",";
        for (const auto& card : visibleCards[i])
        {
            result += card.to_char();
        }
        for (const auto& card : hiddenCards[i])
        {
            result += card.to_char();
        }
        result += ";";
    }
    if (deckCard.empty())
    {
        result += "*";
    } else
    {
        for (const auto& card : deckCard)
        {
            result += card.to_char();
        }
    }
    return result;
}

std::string Poker::to_level_string() const {
    std::vector<std::vector<Card> > hiddenCards;
    std::vector<std::vector<Card> > visibleCards;
    std::vector<Card> deckCard;

    // # 44张隐藏的
    for (int i = 0; i < 44; ++i) {
        const int tmp = i % 10;
        if (hiddenCards.size() <= tmp) {
            hiddenCards.resize(tmp + 1);
        }
        hiddenCards[tmp].push_back(cards[i]);
    }
    for (int i = 44; i < 54; i++) {
        const int tmp = i % 10;
        if (visibleCards.size() <= tmp) {
            visibleCards.resize(tmp + 1);
        }
        visibleCards[tmp].push_back(cards[i]);
    }
    // # 初始化牌堆指针数组
    for (int i = 54; i < cards.size(); i++) {
        deckCard.push_back(cards[i]);
    }
    for (auto& item : hiddenCards) {
        std::ranges::reverse(item);
    }
    if (!reverse_output) {
        // # playvalve 和 pgmaker 的在这里不需要翻转
        std::ranges::reverse(deckCard);
    }


    std::string result;
    for (size_t i = 0; i < 10; i ++)
    {
        for (const auto& card : visibleCards[i])
        {
            result += card.to_char();
        }
        for (const auto& card : hiddenCards[i])
        {
            result += card.to_char();
        }
    }
    for (const auto& card : deckCard)
    {
        result += card.to_char();
    }
    return result;
}

std::ostream & operator<<(std::ostream &out, const Poker &poker) {
    out << poker.get_string();
    return out;
}
