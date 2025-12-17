//
// Created by baizeyv on 4/29/2025.
//

#include "Solver.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ranges>

#include "Const.h"
#include "Helper.h"
#include "exporter/Exporter.h"

std::vector<State *> Solver::take_a_step(State *state, Solver *solver) {
    std::unordered_set<State *, StatePtrHash, StatePtrEqual> results;
    for (size_t i = 0; i < state->visibleCards.size(); i++) {
        // # find all the movable cards
        Card *first = nullptr;
        if (auto movableCards = find_movable_card_in_column(state->visibleCards[i], first); !movableCards.empty()) {
            // # 尝试移动
            for (auto newStates = move_movable_cards(movableCards, i, state, solver); auto &item: newStates) {
                if (!state_exists(results, item))
                    results.insert(item);
                else
                    delete item;
            }
        }
    }
    // # 添加发牌的可能
    constexpr std::vector<Card *> tmp;
    auto newState = create_new_state(state, tmp, -1, -1);
    if (!newState->play_deck())
        return sort(results);
    if (!state_exists(results, newState))
        results.insert(newState);
    else
        delete newState;
    return sort(results);
}

std::vector<Card *> Solver::find_movable_card_in_column(std::vector<Card *> &column, Card *&firstCard) {
    std::vector<Card *> result;
    if (column.empty())
        return result;
    if (firstCard == nullptr) {
        // # 第一张牌指针为空指针
        if (column.size() == 1) {
            // # 此时只有一张可见的牌,则可移动的也就这一张
            result.push_back(column[0]);
            return result;
        }
        // # 可见牌数>1
        result.push_back(column[0]);
        std::vector<Card *> tail;
        for (size_t i = 1; i < column.size(); i++) {
            tail.push_back(column[i]);
        }
        for (const auto list = find_movable_card_in_column(tail, column[0]); auto &card: list) {
            result.push_back(card);
        }
        return result;
    }
    // # 迭代调用
    // # 花色相同且值差为1,此时可以多移动一张
    if (column[0]->suit == firstCard->suit && column[0]->value == firstCard->value + 1) {
        result.push_back(column[0]);
        std::vector<Card *> tail;
        for (size_t i = 1; i < column.size(); i++) {
            tail.push_back(column[i]);
        }
        for (const auto list = find_movable_card_in_column(tail, column[0]); auto &card: list) {
            result.push_back(card);
        }
        return result;
    }
    return result;
}

std::vector<State *> Solver::move_movable_cards(const std::vector<Card *> &movableCards, const int &fromIndex,
                                                State *&state, Solver *&solver) {
    // # 最终结果Set
    std::unordered_set<State *, StatePtrHash, StatePtrEqual> result;
    for (int column = 0; column < state->visibleCards.size(); column++) {
        // # 遍历每一列
        if (column == fromIndex) // # 从自身移动到自身,剪枝
            continue;
        if (state->is_blank(column)) {
            // # 当前状态的column目标列为空
            if (!solver->special_filter) {
                // # 没有启动特殊过滤器时
                for (size_t i = movableCards.size(); i >= 1; i--) {
                    // # 判断是否是空列全部到另一个空列
                    if (i == movableCards.size()) {
                        // # 全移动的情况
                        if (state->visibleCards[fromIndex].size() + state->hiddenCards[fromIndex].size() == i &&
                            state->is_blank(column)) {
                            continue;
                        }
                    }
                    std::vector cards(movableCards.begin(), movableCards.begin() + i);
                    if (!state->visibleCards[column].empty() && !cards.empty() &&
                        state->visibleCards[column][0]->value == cards.back()->value + 1) {
                        // # 可以放到目标列 (符合差值为1的条件)
                        auto newState = create_new_state(state, cards, fromIndex, column);
                        if (!state_exists(result, newState))
                            result.insert(newState);
                        else
                            delete newState;
                    }
                }
            } else {
                // # 启动了过滤器了
                if (movableCards.size() == state->visibleCards[fromIndex].size() &&
                    state->hiddenCards[fromIndex].size() == 0) {
                    // # 当前列没有hidden的牌了，并且要全部移动到另一个空列的情况,不添加
                } else {
                    auto newState = create_new_state(state, movableCards, fromIndex, column);
                    if (!state_exists(result, newState))
                        result.insert(newState);
                    else
                        delete newState;
                }
            }
        } else {
            // # 目标列不为空
            for (size_t i = movableCards.size(); i >= 1; i--) {
                std::vector cards(movableCards.begin(), movableCards.begin() + i);
                if (state->visibleCards[column][0]->value == cards.back()->value + 1) {
                    // # 可以放到目标列 (符合差值为1的条件)
                    auto newState = create_new_state(state, cards, fromIndex, column);
                    if (!state_exists(result, newState))
                        result.insert(newState);
                    else
                        delete newState;
                }
            }
        }
    }
    std::vector<State *> vec(result.begin(), result.end());
    return vec;
}

Solver::Solver(const int seed, const int suitCount, const int max_value, const bool pg_maker) :
    depth(0), sync_end_flag(false), calc(0), solved(false) {
    poker = new Poker(seed, suitCount, max_value, pg_maker);
    root_state = new State(poker);
}

Solver::Solver(const int fake_seed, const std::string &str104, const int max_value) :
    depth(0), sync_end_flag(false), calc(0), solved(false) {
    poker = new Poker(fake_seed, str104, max_value);
    root_state = new State(poker);
}

Solver::Solver(const std::string &vitaLevel) : depth(0), sync_end_flag(false), calc(0), solved(false) {
    poker = new Poker(vitaLevel);
    root_state = new State(poker);
}

Solver::~Solver() {
    all_serialized_states.clear();
    // for (auto& state : all_states)
    // {
    //     delete state;
    // }
    // ! root_state 也在 all_states 中,所以不需要重复释放
    // if (root_state != nullptr) {
    //     delete root_state;
    // }
    delete poker;
}

void Solver::call_step_dfs() {
    depth_first_search_sync(root_state, []() {}, "", 0, false, -1, true);
    Helper::trim_memory();
    // if (!solved)
    // {
    //     all_serialized_states.clear();
    //     special_filter = false;
    //     depth_first_search_sync(root_state, []()
    //     {
    //     }, "", 0, false, -1, true);
    //     special_filter = true;
    //     Helper::trim_memory();
    // }
}

void Solver::call_test_dfs() {
    depth_first_search_sync(root_state, []() {}, "", 0, false, -1);
    Helper::trim_memory();
    // if (!solved)
    // {
    //     all_serialized_states.clear();
    //     special_filter = false;
    //     depth_first_search_sync(root_state, []()
    //     {
    //     }, "", 0, false, -1);
    //     special_filter = true;
    //     Helper::trim_memory();
    // }
}

void Solver::call_dfs(const std::string &file, const int id, const bool exportNull, const int stepLimit) {
    depth_first_search_sync(root_state, []() {}, file, id, exportNull, stepLimit);
    Helper::trim_memory();
    // if (!solved)
    // {
    //     all_serialized_states.clear();
    //     special_filter = false;
    //     depth_first_search_sync(root_state, []()
    //     {
    //     }, file, id, exportNull, stepLimit);
    //     special_filter = true;
    //     Helper::trim_memory();
    // }
}

void Solver::depth_first_search_sync(State *&root, const std::function<void()> &onCompleted, const std::string &file,
                                     const int id, const bool exportNull, const int stepLimit, const bool step_mode) {
    if (step_mode) {
        while (next_step == 0) {
            if (abort_step == 1) {
                sync_end_flag = true;
                break;
            }
        }
        next_step = 0;
    }
    depth++;
    calc++;
    all_serialized_states.insert(root->to_serialized());
    if (calc % 1000000 == 0) // # 每1000000次尝试就释放一次物理内存,防止垃圾机子爆内存
        Helper::trim_memory();
    // all_states.insert(root);
    root->calc = calc;
    if (prepare_query == 1) {
        std::cout << "\b\b" << *root << "> ";
        prepare_query = 0;
    }
    if (step_mode && abort_step == 0) {
        std::cout << "\b\b" << *root << std::endl << "> ";
    }
    // std::cout << root->get_memory_usage() << std::endl;
    if (spd::DebugOutput) {
        if (spd::OutputPath.empty()) {
            spd::OutputPath = Helper::get_current_exe_directory();
        }
        std::string suffix = "\\debug_log\\";
        if (int tmp; Helper::try_parse_int(poker->mark, tmp)) {
            // playvalve
            suffix += "playvalve_" + std::to_string(tmp) + ".log";
        } else {
            // # vita
            suffix += "vita_" + poker->mark.substr(0, 20) + ".log";
        }
        const std::string debug_output_file = spd::OutputPath + suffix;
        Helper::check_file_and_create_dir_when_needed(debug_output_file);

        if (calc < 1 && std::filesystem::exists(debug_output_file)) {
            std::filesystem::remove(debug_output_file);
        }
        std::ofstream writer(debug_output_file, std::ios::app);
        writer << *root << "\n";
    }

    // # 在当前合理的可能步骤数组中找到没有试过的扑克状态
    std::vector<State *> states;
    for (auto no_filter_states = take_a_step(root, this); auto &item: no_filter_states) {
        // if (!state_exists(all_states, item) && item->secondary_valuation(this))
        if (!state_serialized_exists(all_serialized_states, item) && item->secondary_valuation(this)) {
            states.push_back(item);
        } else {
            delete item;
        }
    }
    // std::cout << std::to_string(states.size()) << " !!! " << std::endl;
    if ((calc >= stepLimit && stepLimit > 0) || depth >= 1000) {
        // # 超出步骤限制了
        if (!file.empty() && exportNull) {
            // # Export null
            const auto exporter = new Exporter(file);
            exporter->export_csv(id, *root, true);
            delete exporter;
        }
        sync_end_flag = true;
        // ! >= 1000 防止 StackOverflow
        // # 如果调用栈达到这么多的时候,用这个算法基本上就不好求解了,直接结束
        return;
    }
    // ! delete root; // ! 不能在这里删除,因为State内部使用了上一步的State,只有在剪枝的时候才时候delete
    // # 完成后需要continue去delete state pointer
    bool completed_continue_flag = false;
    if (step_mode && abort_step == 1)
        completed_continue_flag = true;
    // # 遍历所有没有试过的状态
    for (size_t i = 0; i < states.size(); i++) {
        if (completed_continue_flag) {
            delete states[i];
            continue;
        }
        if (states[i]->is_completed()) {
            // # 完成游戏
            // std::cout << "Game Completed !!!" << std::endl;
            solved = true;
            states[i]->calc = calc;
            onCompleted();
            if (!file.empty()) {
                const auto exporter = new Exporter(file);
                exporter->export_csv(id, *states[i]);
                delete exporter;
            }
            sync_end_flag = true;
            completed_continue_flag = true;
            delete states[i];
            continue;
        }
        for (size_t x = i; x < states.size(); x++) {
            all_serialized_states.insert(states[x]->to_serialized());
            // all_states.insert(item);
        }
        // for (const auto& item : states)
        // {
        //     all_serialized_states.insert(item->to_serialized());
        //     // all_states.insert(item);
        // }
        depth_first_search_sync(states[i], onCompleted, file, id, exportNull, stepLimit, step_mode);
        depth--;
        if (sync_end_flag || abort_step == 1) {
            completed_continue_flag = true;
        }
        delete states[i];
    }
}

void Solver::stop() { sync_end_flag = true; }

State *Solver::create_new_state(const State *state, const std::vector<Card *> &cards, const int fromIndex,
                                const int toIndex) {
    const auto resultState = new State(state);
    if (fromIndex < 0 || toIndex < 0)
        return resultState;
    resultState->move_card(fromIndex, cards.size(), toIndex);
    return resultState;
}

bool Solver::state_exists(std::unordered_set<State *, StatePtrHash, StatePtrEqual> &results, State *&newState) {
    return results.contains(newState);
}

bool Solver::state_serialized_exists(const std::unordered_set<std::string> &results, State *&newState) {
    const auto str = newState->to_serialized();
    return results.contains(str);
}

std::vector<State *> Solver::sort(std::unordered_set<State *, StatePtrHash, StatePtrEqual> states) {
    std::vector<State *> list(states.begin(), states.end());
    // # 第一层,按Valuation降序排序
    std::ranges::stable_sort(list, [](State *a, State *b) {
        const int av = a->get_valuation();
        const int bv = b->get_valuation();
        // return av > bv;
        if (av > bv)
            return true;
        if (av < bv)
            return false;
        if (av == bv) {
            const auto af = a->history[0].get_from();
            const auto ac = a->history[0].get_count();
            const auto at = a->history[0].get_to();

            const auto bf = b->history[0].get_from();
            const auto bc = b->history[0].get_count();
            const auto bt = b->history[0].get_to();
            if (af < 0 || ac < 0 || at < 0) {
                // # a是发牌
                return false;
            }
            if (bf < 0 || bc < 0 || bt < 0) {
                // # b是发牌
                return true;
            }
            if (af == bf) {
                return at < bt;
            } else {
                return false;
            }
        }
        return false;
    });
    // # 第二层,再根据指定规则排序
    std::ranges::stable_sort(list, [](State *a, State *b) {
        auto getPriority = [](State *p) -> int {
            if (!p->previous || p->get_suit_count() <= 1)
                return std::numeric_limits<int>::min();

            const auto from = p->history[0].get_from();
            const auto to = p->history[0].get_to();
            if (from < 0 || to < 0 || from < to)
                return std::numeric_limits<int>::min();
            const auto &prev = p->previous;
            if (!prev->visibleCards[to].empty() &&
                prev->visibleCards[from][0]->suit == prev->visibleCards[to][0]->suit) {
                return std::numeric_limits<int>::max();
            }
            return std::numeric_limits<int>::min();
        };
        return getPriority(a) > getPriority(b);
    });
    return list;
}
