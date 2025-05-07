//
// Created by baizeyv on 4/29/2025.
//

#ifndef STATE_H
#define STATE_H
#include <memory>
#include <vector>

#include "Card.h"
#include "HistoryItem.h"
#include "IMemUsage.h"
#include "Poker.h"
// #include "Solver.h"
class Solver;


class State final : public IMemUsage {

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

    explicit State(const State *previous_state);

    explicit State(Poker * &poker);

    ~State() override;

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

    double evaluate();

    /**
     * * 计算估值
     * @return 估值结果
     */
    double calculate_valuation();

    /**
     * * 一次估值,用于估计当前整个牌面的优先级
     * @return
     */
    int get_valuation(const Solver *solver) ;

    /**
     * * 二次估值,用于将无效移动的优先级降低,例如 0:KQJ 1:K 将0:QJ移动到1
     * @param solver
     * @return
     */
    bool secondary_valuation(const Solver* solver) const;

    /**
     * * 三次估值,用于处理向空列移动相关的部分
     * # 1. 用空列移动部分组来合并为同花色组 + 10
     * # 2. 移动后能翻开面朝下的牌 (可能已经通过FlopValuation处理过了) + 8
     * # 3. 移动完整的同花色组 + 6
     * # 4. 产生新空列 + 5
     * # 5. 移动破坏同花色组 - 10
     * # 6. 向空列放入K (除非计划重建) - 5
     * @return 三次估值
     */
    int third_valuation() const;

    /**
     * * 获取花色数量
     * @return
     */
    int get_suit_count() const;

    std::string to_string() const;

    std::string to_full_string() const;

    friend std::ostream& operator<<(std::ostream& out, const State& state);

    size_t get_memory_usage() const override;

    /**
     * * 获取序列化后的字符串,用于保存State,节省内存
     * @return 
     */
    std::string to_serialized() const;

private:
    int valuation = -9999;

    double evaluate_valuation = -9999;

    /**
     * * 检测是否可以收集一套牌
     * @param index
     * @return
     */
    bool detect_collection(int index);

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

    std::string hidden_string(int row, int max) const;

    std::string floor_hidden_string(int row)const;

    std::string visible_string(int row, int max) const;

    std::string floor_visible_string(int row)const;

    std::string deck_string() const;

    /**
     * * 找到指定列牌的可以移动的部分
     * @param cards
     * @return
     */
    static std::vector<Card *> find_movable_cards_in_columns(const std::vector<Card *>& cards);
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
