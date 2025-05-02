//
// Created by baizeyv on 4/29/2025.
//

#ifndef STATE_H
#define STATE_H
#include <vector>

#include "Card.h"
#include "HistoryItem.h"
#include "Poker.h"
// #include "Solver.h"
class Solver;


class State {
public:
    const Poker* poker;
    /**
     * * 牌堆指针数组
     */
    std::vector<Card *> deckCard;

    /**
     * * 隐藏牌指针数组
     */
    std::vector<std::vector<Card *> > hiddenCards;

    /**
     * * 可见牌指针数组
     */
    std::vector<std::vector<Card *> > visibleCards;

    /**
     * * 历史记录
     */
    std::vector<HistoryItem> history;

    /**
     * * 收牌的步骤记录
     */
    std::vector<int> collection_steps;

    /**
     * * 上一步状态
     */
    const State *previous;

    int calc = 0;

    int card_count = 104;

    State(const State *previous_state);

    State(Poker * &poker);

    /**
     * * 完成了几套牌了
     * @return 完成的牌组数
     */
    int finished_count() const;

    /**
     * * 当前游戏状态是否完成了
     * @return
     */
    bool is_completed() const;

    /**
     * * 指定列是否为空
     * @param index
     * @return
     */
    bool is_blank(int index) const;

    /**
     * * 移动牌
     * @param from
     * @param count
     * @param to
     */
    void move_card(int from, int count, int to);

    /**
     * * 发牌
     * @return
     */
    bool play_deck();

    /**
     * * 一次估值
     * @return
     */
    int get_valuation() ;

    bool secondary_valuation(const Solver* solver);

    /**
     * * 获取花色数量
     * @return
     */
    int get_suit_count() const;

    std::string to_string() const;

    std::string to_full_string() const;

    friend std::ostream& operator<<(std::ostream& out, const State& state);

private:
    int valuation = -9999;

    std::vector<int> columnValuation{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /**
     * * 检测是否可以收集一套牌
     * @param index
     * @return
     */
    bool detect_collection(const int index);

    /**
     * * 翻牌额外估值
     * @param limit
     * @param divide
     * @return
     */
    int flop_valuation(int limit, bool divide) const;

    int check_flop(const State* state, int column, int& depth, int limit) const;

    /**
     * * 多花色的向空列移动的情况的额外估值 (可以整理牌型)
     * @return
     */
    int extra_valuation_more_suit() const;

    /**
     * * 获取空白列的数量
     * @return
     */
    int blank_column_count() const;

    std::string hidden_string(const int row, const int max) const;

    std::string floor_hidden_string(const int row)const;

    std::string visible_string(const int row, const int max) const;

    std::string floor_visible_string(const int row)const;

    std::string deck_string() const;
};

/**
 * * 自定义Hash器
 */
struct StatePtrHash {
    std::size_t operator()(const State* s) const {
        int hash = 17;
        for (auto& item : s->deckCard) {
            hash = hash * 31 + item->original_value;
        }
        for (auto& group : s->hiddenCards) {
            for (auto& item : group) {
                hash = hash * 31 + item->original_value;
            }
        }
        for (auto& group : s->visibleCards) {
            for (auto& item : group) {
                hash = hash * 31 + item->original_value;
            }
        }
        return hash;
    }
};

struct StatePtrEqual {
    bool operator()(const State* lhs, const State* rhs) const {
        if (lhs->deckCard.size() != rhs->deckCard.size()) {
            return false;
        }
        for (size_t i = 0; i < 10; i ++) {
            if (lhs->visibleCards.size() != rhs->visibleCards.size()) {
                return false;
            }
            if (lhs->hiddenCards.size() != rhs->hiddenCards.size()) {
                return false;
            }
            for (size_t x = 0; x < lhs->visibleCards[i].size(); x ++) {
                if (lhs->visibleCards[i].size() != rhs->visibleCards[i].size()) {
                    return false;
                }
                if (lhs->visibleCards[i][x]->suit != rhs->visibleCards[i][x]->suit) {
                    return false;
                }
                if (lhs->visibleCards[i][x]->value != rhs->visibleCards[i][x]->value) {
                    return false;
                }
            }
            for (size_t x = 0; x < lhs->hiddenCards[i].size(); x ++) {
                if (lhs->hiddenCards[i].size() != rhs->hiddenCards[i].size()) {
                    return false;
                }
                if (lhs->hiddenCards[i][x]->suit != rhs->hiddenCards[i][x]->suit) {
                    return false;
                }
                if (lhs->hiddenCards[i][x]->value != rhs->hiddenCards[i][x]->value) {
                    return false;
                }
            }
        }
        return true;
    }
};

#endif //STATE_H
