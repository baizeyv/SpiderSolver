//
// Created by baizeyv on 5/8/2025.
//

#include "calculator.h"

#include <algorithm>
#include <set>

double spd::calculator::calculate_valuation_in_column(const vector<Card *> &cards) {
    // # 最终估值结果
    double ret = 0.0;
    if (cards.empty())
        return ret;
    const Card* top_card = cards.front();
    // * 序列计数器
    int sequence_counter = 0;
    for (size_t i = 1; i < cards.size(); ++i) {
        top_card = cards[i];
        Card* bottom_card = cards[i - 1];
        if (top_card->value == bottom_card->value + 1) {
            // # 点数相差1
            if (top_card->suit == bottom_card->suit) {
                // # 同花色
                sequence_counter ++;
            } else {
                // # 异花色
                // * 对之前的同色序列进行估值叠加
                add_value(sequence_counter, bottom_card->value, ret);
                // * 对当前这个阻断点进行估值叠加
                add_value(1, 1, ret);
                // * 重置序列计数器
                sequence_counter = 0;
            }
        } else {
            // # 点数不是差1
            // * 对之前的同色序列进行估值叠加
            add_value(sequence_counter, bottom_card->value, ret);
            // * 重置序列计数器
            sequence_counter = 0;

            //一个乱序组
            //eg. 7 1 -> -7
            //eg. 1 7 -> -14
            //eg. 5 5 -> -10
            double dv = -std::ranges::max(top_card->value, bottom_card->value);
            if (top_card->value < bottom_card->value) {
                dv *= 2;
            }
            add_value(1, dv, ret);
        }
    }
    // * 累加最上边一张的可见牌
    add_value(sequence_counter, top_card->value, ret);
    return ret;
}

double spd::calculator::calculate_penalty_in_hidden_column(const vector<Card *> &cards) {
    double ret = 0.0;
    if (cards.empty())
        return ret;
    // # 没有翻开的牌值: -10, -9, -8, -7, -6, -5
    int num = 10;
    for (const auto& _ : cards) {
        ret -= num;
        num --;
    }
    return ret;
}

bool spd::calculator::detect_indirect_collection(const vector<vector<Card *>> &visible_cards) {
    std::set<char> card_set;
    for (const auto& card_groups : visible_cards) {
        for (const auto& card : find_movable_cards_in_column(card_groups)) {
            card_set.insert(card->to_char());
        }
    }
    bool suit_1_flag = true;
    for (char c = 'a'; c < 'a' + 13; ++c) {
        if (!card_set.contains(c)) {
            suit_1_flag = false;
            break;
        }
    }
    if (suit_1_flag)
        return true;
    bool suit_2_flag = true;
    for (char c = 'a' + 13; c < 'a' + 26; ++c) {
        if (!card_set.contains(c)) {
            suit_2_flag = false;
            break;
        }
    }
    if (suit_2_flag)
        return true;
    bool suit_3_flag = true;
    for (char c = 'A'; c < 'A' + 13; ++ c) {
        if (!card_set.contains(c)) {
            suit_3_flag = false;
            break;
        }
    }
    if (suit_3_flag)
        return true;
    bool suit_4_flag = true;
    for (char c = 'A' + 13; c < 'A' + 26; ++ c) {
        if (!card_set.contains(c)) {
            suit_4_flag = false;
            break;
        }
    }
    if (suit_4_flag)
        return true;
    return false;
}

vector<Card *> spd::calculator::find_movable_cards_in_column(const vector<Card *> &cards) {
    // # 最终结果:在当前列中可移动的牌
    vector<Card*> ret;
    if (cards.empty())
        return ret;
    Card* top_card = cards.front();
    ret.push_back(top_card);
    for (size_t i = 1; i < cards.size(); ++i) {
        top_card = cards[i];
        const Card* bottom_card = cards[i - 1];
        if (top_card->value == bottom_card->value + 1 && top_card->suit == bottom_card->suit) {
            // # 达成同花色的序列
            ret.push_back(top_card);
        } else {
            break;
        }
    }
    return ret;
}

bool spd::calculator::detect_form_blank_with_help_of_blank(const State *state) {
    if (state->previous == nullptr)
        return false;
    const int to_index = state->history[0].get_to();
    const int from_index = state->history[0].get_from();
    if (to_index < 0 || from_index < 0) {
        // # 排除发牌的情况
        return false;
    }
    if (state->previous->is_blank(to_index)) {
        // # 移动序列到空列了
        // * 当前from列可移动的牌序列
        const auto movable_from = find_movable_cards_in_column(state->visibleCards[from_index]);
        if (state->visibleCards[from_index].size() == movable_from.size()) {
            // # 新暴露出的from列覆盖牌可以全部移动的情况
            if (state->hiddenCards[from_index].empty()) {
                // # 当前from列没有隐藏牌了
                for (size_t i = 0; i < state->visibleCards.size(); ++i) {
                    // # 遍历所有可见牌
                    if (i == from_index)
                        // # 跳过起始位置
                        continue;
                    if (state->visibleCards[i].empty())
                        // # 跳过空列
                        continue;
                    if (state->visibleCards[i][0]->value - 1 == movable_from.back()->value) {
                        // # 差值为1,可以移动到新位置 (不判断花色)
                        return true; // 可以构建出新列
                    }
                }
            } else {
                // # 全部移动后会出新的隐藏牌的情况
                if (bool allow_use_other_blank = false; state->blank_prediction_dfs(from_index, to_index, allow_use_other_blank))
                    return true;
            }
        } else {
            // # 新暴露出的from列的覆盖牌不能全部移动,此时需要判断其他列是否可以借助这一列来构建出新的空列
            if (bool allow_use_other_blank = false; state->blank_prediction_dfs(from_index, to_index, allow_use_other_blank))
                return true;
        }
    } else {
        // ! 这个部分就不属于借助空列来形成空列了,属于可构成空列的范畴
        // # 移动序列到了非空列,需要判断是否可以构建出新的空列
        // TODO: 需要一个新的dfs方法
        // * 这部分需要移动到序列移动的方法中
    }
    return false;
}

bool spd::calculator::detect_form_blank_without_blank(const State *state) {
    if (state->previous == nullptr)
        return false;
    const int to_index = state->history[0].get_to();
    const int from_index = state->history[0].get_from();
    if (to_index < 0 || from_index < 0) {
        // # 排除发牌的情况
        return false;
    }
    if (!state->previous->is_blank(to_index)) {
        // # 新暴露出的from列的覆盖牌不能全部移动,此时需要判断其他列是否可以借助这一列来构建出新的空列
        if (bool allow_use_other_blank = true; state->blank_prediction_dfs(from_index, to_index, allow_use_other_blank))
            return true;
    }
    return false;
}

void spd::calculator::add_value(const int &num, const int &top_point, double &result) {
    if (num != 0)
        result += top_point * num;
}
