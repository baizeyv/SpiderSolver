//
// Created by baizeyv on 12/16/2025.
//

#include "Joker.h"

#include <iostream>

#include "Const.h"
#include "Helper.h"
#include "State.h"
Joker::Joker(const std::string &level_serialized) {
    const auto array = Helper::split(level_serialized, ";");
    // # deck 50 张
    for (const auto deck_string_list = array[array.size() - 1]; const auto &c: deck_string_list) {
        const auto cd = Card(c);
        this->deck_cards.push_back(cd);
    }

    for (size_t i = 0; i < 10; ++i) {
        this->visible_cards.emplace_back();
        this->hidden_cards.emplace_back();
    }

    for (size_t i = 0; i < array.size() - 1; ++i) {
        const auto tmp = Helper::split(array[i], ",");
        // # 可见牌的数量
        int visible_count;
        Helper::try_parse_int(tmp[0], visible_count);
        for (int t = 0; t < visible_count; ++t) {
            const auto c = tmp[1][t];
            const auto cd = Card(c);
            this->visible_cards[i].push_back(cd);
        }
        // # 不可见的牌
        for (int t = visible_count; t < tmp[1].size(); ++t) {
            const auto c = tmp[1][t];
            const auto cd = Card(c);
            this->hidden_cards[i].push_back(cd);
        }
    }
}

int Joker::get_first_movable_count() {
    int count = 0;
    std::vector<int> list{};
    for (size_t i = 0; i < visible_cards.size(); ++i) {
        list.push_back(visible_cards[i][0].value);
    }
    for (int i = 0; i < visible_cards.size(); ++i) { // # from
        for (int j = 0; j < visible_cards.size(); ++j) { // # to
            if (j == i)
                continue;
            if (list[i] + 1 == list[j])
                count++;
        }
    }
    // for (size_t i = 0; i < this->visible_cards.size(); ++i) {
    //     // # find all the movable cards
    //     Card *first = nullptr;
    //     auto movableCards = find_movable_card_in_column(this->visible_cards[i], first);
    //     if (!movableCards.empty()) {
    //         // # 有可以移动的牌
    //     }
    //     count += movableCards.size();
    // }
    if (!deck_cards.empty()) {
        // # 添加发牌的这一次
        count++;
    }
    return count;
}

int Joker::get_first_empty_column_count_by_state(const std::string &history) {
    if (history.size() <= 10) {
        // # [UNSOLVED]
        return -1;
    }
    int result = 0;
    const auto array = Helper::split(history, ">");

    std::vector<Card> ddd = deck_cards;
    std::vector<std::vector<Card>> hhh = hidden_cards;
    std::vector<std::vector<Card>> vvv = visible_cards;

    State st(vvv, hhh, ddd);

    for (const auto &item: array) {
        if (item.empty())
            continue;
        result++;
        const auto list = Helper::split(item, ":");
        int from_column_index;
        Helper::try_parse_int(Helper::split(list[1], ",")[0], from_column_index);
        int to_column_index;
        Helper::try_parse_int(Helper::split(list[2], ",")[0], to_column_index);
        int motion_count;
        Helper::try_parse_int(Helper::split(list[3], "]")[0], motion_count);

        if (from_column_index < 0 || to_column_index < 0 || motion_count < 0) {
            // # 需要发牌了
            st.play_deck();
        } else {
            st.move_card(from_column_index, motion_count, to_column_index);
        }
        for (int i = 0; i < 10; i++) {
            if (st.is_blank(i)) {
                return result;
            }
        }
    }
    return -1;
}

std::vector<Card> Joker::find_movable_card_in_column(std::vector<Card> &column, Card *&first_card) {
    std::vector<Card> result;
    if (column.empty())
        return result;
    if (first_card == nullptr) {
        // # 第一张牌为空
        if (column.size() == 1) {
            // # 此时只有一张可见的牌,则可移动的也就这一张
            result.push_back(column[0]);
            return result;
        }
        // # 可见牌数>1
        result.push_back(column[0]);
        std::vector<Card> tail;
        for (size_t i = 1; i < column.size(); i++) {
            tail.push_back(column[i]);
        }
        Card *tmp = &column[0];
        for (const auto list = find_movable_card_in_column(tail, tmp); auto &card: list) {
            result.push_back(card);
        }
    }
    // # 迭代调用
    // # 花色相同且值差为1,此时可以多移动一张
    if (column[0].suit == first_card->suit && column[0].value == first_card->value + 1) {
        result.push_back(column[0]);
        std::vector<Card> tail;
        for (size_t i = 1; i < column.size(); i++) {
            tail.push_back(column[i]);
        }
        auto tmp = &column[0];
        for (const auto list = find_movable_card_in_column(tail, tmp); auto &card: list) {
            result.push_back(card);
        }
        return result;
    }
    return result;
}

std::string Joker::to_str(const std::vector<std::vector<Card>> &v_cards, std::vector<std::vector<Card>> &h_cards,
                          std::vector<Card> &deck) {
    std::string res;
    int max_hidden = 0;
    for (auto &item: h_cards) {
        if (!item.empty() && item.size() > max_hidden) {
            max_hidden = item.size();
        }
    }
    int max_visible = 0;
    for (auto &item: v_cards) {
        if (!item.empty() && item.size() > max_visible) {
            max_visible = item.size();
        }
    }
    res += hidden_string(0, max_hidden, h_cards);
    res += "\n";
    res += "\n";
    res += visible_string(0, max_visible, v_cards);
    res += "\n";
    res += "\n";
    res += deck_string(deck);

    return res;
}

std::string Joker::hidden_string(const int row, const int max, const std::vector<std::vector<Card>> &hhh) {
    if (max == 0)
        return "";
    if (row == max - 1)
        return floor_hidden_string(row, hhh);
    return floor_hidden_string(row, hhh) + "\n" + hidden_string(row + 1, max, hhh);
}

std::string Joker::floor_hidden_string(const int row, const std::vector<std::vector<Card>> &hhh) {
    std::string result;
    for (auto &column: hhh) {
        if (column.size() > row) {
            result += column[column.size() - row - 1].to_string();
        } else {
            result += spd::EmptyCard;
        }
    }
    return result;
}

std::string Joker::visible_string(const int row, const int max, const std::vector<std::vector<Card>> &vvv) {
    if (max == 0)
        return "";
    if (row == max - 1)
        return floor_visible_string(row, vvv);
    return floor_visible_string(row, vvv) + "\n" + visible_string(row + 1, max, vvv);
}

std::string Joker::floor_visible_string(const int row, const std::vector<std::vector<Card>> &vvv) {
    std::string result;
    for (auto &column: vvv) {
        if (column.size() > row) {
            result += column[column.size() - row - 1].to_string();
        } else {
            result += spd::EmptyCard;
        }
    }
    return result;
}

std::string Joker::deck_string(const std::vector<Card> &ddd) {
    std::string result;
    for (size_t i = 0; i < ddd.size(); i++) {
        if (i % 10 == 0 && i != 0) {
            result += "\n";
        }
        result += ddd[i].to_string();
    }
    return result;
}
