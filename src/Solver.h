//
// Created by baizeyv on 4/29/2025.
//

#ifndef SOLVER_H
#define SOLVER_H
#include <atomic>
#include <functional>
#include <unordered_set>

#include "State.h"


class Solver {
private:

    Poker* poker;

    State* root_state;

    /**
     * * 所有尝试过的状态的指针HashSet
     */
    std::unordered_set<State*, StatePtrHash, StatePtrEqual> all_states;

    /**
     * * 求解深度
     */
    int depth;

    std::atomic<bool> sync_end_flag;

    static std::vector<State *> take_a_step(State* state, Solver* solver);

    /**
     * * 找到一列中可以移动的牌指针数组
     * @param column
     * @param firstCard
     * @return
     */
    static std::vector<Card*> find_movable_card_in_column(std::vector<Card*>& column, Card*& firstCard);

    static std::vector<State*> move_movable_cards(const std::vector<Card*>& movableCards, const int& fromIndex, State*& state, Solver*& solver);

public:
    /**
     * * 特殊算法过滤器,当使用过滤器无解时再关闭过滤器重新执行一次
     * true->代表开启过滤器
     */
    bool special_filter = true;

    State* current_state;

    /**
     * * 尝试求解总次数
     */
    int calc;

    bool solved;

    explicit Solver(int seed, int suitCount);

    explicit Solver(const std::string &vitaLevel);

    ~Solver();

    void call_test_dfs();

    void depth_first_search_sync(State* &root, const std::function<void()> &onCompleted, const std::string &file = "", int id = 0, bool exportNull = true, int stepLimit = -1);

    void stop();

    /**
     * * 创建新的Poker状态
     * @param state
     * @param cards
     * @param fromIndex
     * @param toIndex
     * @return
     */
    static State* create_new_state(const State* state, const std::vector<Card*>& cards, const int fromIndex, const int toIndex);

    static bool state_exists(std::unordered_set<State *, StatePtrHash, StatePtrEqual>& results, State* &newState);

    static std::vector<State *> sort(std::unordered_set<State *, StatePtrHash, StatePtrEqual> states);
};



#endif //SOLVER_H
