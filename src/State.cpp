//
// Created by baizeyv on 4/29/2025.

#include "State.h"

#include <algorithm>
#include <unordered_set>

#include "Const.h"
#include "Helper.h"
#include "Solver.h"

State::State(Poker * &poker) {
    this->poker = poker;
    previous = nullptr;

    std::vector<Card> tmpHidden(poker->cards.begin(), poker->cards.begin() + 44);

    // # 44张隐藏的
    for (int i = 0; i < 44; ++i) {
        const int tmp = i % 10;
        if (hiddenCards.size() <= tmp) {
            hiddenCards.resize(tmp + 1);
        }
        hiddenCards[tmp].push_back(&poker->cards[i]);
    }
    for (int i = 44; i < 54; i++) {
        const int tmp = i % 10;
        if (visibleCards.size() <= tmp) {
            visibleCards.resize(tmp + 1);
        }
        visibleCards[tmp].push_back(&poker->cards[i]);
    }
    // # 初始化牌堆指针数组
    for (int i = 54; i < poker->cards.size(); i++) {
        deckCard.push_back(&poker->cards[i]);
    }
    for (auto &item: hiddenCards) {
        std::ranges::reverse(item);
    }
    if (int seed; !Helper::try_parse_int(poker->mark, seed)) {
        // # vita 的种子, 这个时候需要翻转 deckCards
        std::ranges::reverse(deckCard);
    }
}

State::~State() {
    deckCard.clear();
    hiddenCards.clear();
    visibleCards.clear();
    hiddenCards.clear();
    collection_steps.clear();
}

State::State(const State *previous_state) {
    std::vector<std::vector<Card *> > newVisibleCards;
    std::vector<std::vector<Card *> > newHiddenCards;
    const std::vector newDeckCards(previous_state->deckCard.begin(), previous_state->deckCard.end());
    const std::vector newHistory(previous_state->history.begin(), previous_state->history.end());
    for (int i = 0; i < 10; i++) {
        std::vector x(previous_state->visibleCards[i].begin(), previous_state->visibleCards[i].end());
        newVisibleCards.push_back(x);
        std::vector y(previous_state->hiddenCards[i].begin(), previous_state->hiddenCards[i].end());
        newHiddenCards.push_back(y);
    }
    visibleCards = newVisibleCards;
    hiddenCards = newHiddenCards;
    deckCard = newDeckCards;
    history = newHistory;
    if (deckCard.size() > 50)
        throw std::string("Error On `State Constructor` -> Deck Cards Count Must Be <= 50");
    previous = previous_state;
    card_count = previous_state->card_count;
    collection_steps = previous_state->collection_steps;
    poker = previous_state->poker;
}


int State::finished_count() const {
    return 8 - (card_count / 13);
}

bool State::is_completed() const {
    if (!deckCard.empty())
        return false;
    for (auto &item: hiddenCards) {
        if (!item.empty())
            return false;
    }

    for (auto &item: visibleCards) {
        if (!item.empty())
            return false;
    }
    return true;
}

bool State::is_blank(const int index) const {
    return visibleCards[index].size() + hiddenCards[index].size() == 0;
}

void State::move_card(const int from, const int count, const int to) {
    const size_t tmpCount = std::min<size_t>(count, visibleCards[from].size());
    for (int i = tmpCount - 1; i >= 0; i--) {
        visibleCards[to].insert(visibleCards[to].begin(), visibleCards[from][i]);
    }
    visibleCards[from].erase(visibleCards[from].begin(), visibleCards[from].begin() + tmpCount);
    // # 检测收牌
    bool collection = detect_collection(to);
    // # 来源列再没有可见牌的时候要翻开hidden牌
    if (visibleCards[from].empty() && !hiddenCards[from].empty()) {
        visibleCards[from].insert(visibleCards[from].begin(), std::make_move_iterator(hiddenCards[from].begin()),
                                  std::make_move_iterator(hiddenCards[from].begin() + 1));
        hiddenCards[from].erase(hiddenCards[from].begin(), hiddenCards[from].begin() + 1);
    }
    // # 添加历史记录
    auto cur = HistoryItem();
    cur.set_from(from);
    cur.set_count(count);
    cur.set_to(to);
    cur.set_collection(collection);
    history.insert(history.begin(), cur);
}

bool State::play_deck() {
    if (deckCard.empty())
        return false;
    bool collection = false;
    for (size_t i = 0; i < 10; i++) {
        visibleCards[i].insert(visibleCards[i].begin(), std::make_move_iterator(deckCard.begin()),
                               std::make_move_iterator(deckCard.begin() + 1));
        deckCard.erase(deckCard.begin(), deckCard.begin() + 1);
        collection |= detect_collection(i);
    }
    // # 添加发牌历史记录
    auto cur = HistoryItem();
    cur.set_from(-1);
    cur.set_count(-1);
    cur.set_to(-1);
    cur.set_collection(collection);
    history.insert(history.begin(), cur);
    return true;
}

int State::get_valuation() {
    if (valuation != -9999)
        return valuation;
    auto addValue = [](const int num, const int topPoint, int &result) {
        if (num != 0)
            result += topPoint * num;
    };
    // # 完成一套牌 +200 分
    int value = finished_count() * 200;
    for (size_t i = 0; i < hiddenCards.size(); i++) {
        // # 没有翻开的牌值: -10, -9, -8, -7, -6, -5
        // # 未翻开牌减分机制
        int num = 10;
        for (auto &_: hiddenCards[i]) {
            value -= num;
            num--;
        }
        int tmp = value;
        if (!visibleCards[i].empty()) {
            int val = 0;
            auto top = visibleCards[i][0];
            for (size_t x = 1; x < visibleCards[i].size(); x++) {
                top = visibleCards[i][x];
                auto down = visibleCards[i][x - 1];
                // # 点数相差1
                if (top->value == down->value + 1) {
                    if (top->suit == down->suit) {
                        // # 花色相同
                        val++;
                    } else {
                        // # 花色不同
                        addValue(val, down->value, value);
                        addValue(1, 1, value);
                        val = 0;
                    }
                } else {
                    addValue(val, down->value, value);
                    val = 0;
                    //一个乱序组
                    //eg. 7 1 -> -7
                    //eg. 1 7 -> -14
                    //eg. 5 5 -> -10
                    int dv = -std::ranges::max(top->value, down->value);
                    if (top->value < down->value)
                        dv *= 2;
                    addValue(1, dv, value);
                }
            }
            addValue(val, top->value, value);
        }
        columnValuation[i] = value - tmp;
    }
    const int flop = flop_valuation(6, false);
    const int extra = extra_valuation_more_suit();
    valuation = value + flop + extra;
    return valuation;
}

bool State::secondary_valuation(const Solver *solver) {
    if (previous == nullptr)
        return true;
    const auto from = history[0].get_from();
    const auto count = history[0].get_count();
    const auto to = history[0].get_to();
    const auto collection = history[0].get_collection();
    if (from < 0 || count < 0 || to < 0)
        // # 发牌
        return true;
    if (previous->is_blank(to))
        // # 目标列为空列
        return true;
    if (previous->visibleCards[from].size() == count || collection)
        // # 一列去不都移动或收牌了
        return true;
    if (!solver->special_filter)
        // # 不启动过滤器的不进行二次估值
        return true;
    auto calculate = [](const std::vector<Card *> &cards) {
        auto addValue = [](const int num, const int topPoint, int &result) {
            if (num != 0)
                result += topPoint * num;
        };
        int value = 0;
        int val = 0;
        for (size_t i = 1; i < cards.size(); i++) {
            auto top = cards[i];
            auto down = cards[i - 1];
            if (top->value == down->value + 1) {
                if (top->suit == down->suit) {
                    val++;
                    if (i + 1 == cards.size()) {
                        addValue(val, top->value, value);
                    }
                } else {
                    addValue(val, down->value, value);
                    break;
                }
            } else {
                addValue(val, down->value, value);
                break;
            }
        }
        return value;
    };
    const int previousValue = calculate(previous->visibleCards[from]);
    const int currentValue = calculate(visibleCards[to]);
    return currentValue > previousValue;
}

int State::get_suit_count() const {
    return poker->suitCount;
}

std::string State::to_string() const {
    std::string res;
    int max_hidden = 0;
    for (auto &item: hiddenCards) {
        if (!item.empty() && item.size() > max_hidden) {
            max_hidden = item.size();
        }
    }
    int max_visible = 0;
    for (auto &item: visibleCards) {
        if (!item.empty() && item.size() > max_visible) {
            max_visible = item.size();
        }
    }
    res += hidden_string(0, max_hidden);
    res += "\n";
    res += "\n";
    res += visible_string(0, max_visible);
    res += "\n";
    res += "\n";
    res += deck_string();

    return res;
}

std::string State::to_full_string() const {
    std::string result =
            "[-----------------------------------------------------------------------------------------------------]\n";
    result += "[CALC: " + std::to_string(calc) + "] [VALUATION: " + std::to_string(valuation) + "] [STEP: " +
            std::to_string(history.size()) + "] [COLLECTION: " + std::to_string(finished_count()) + "] ";
    if (previous) {
        // # 存在上一步
        const auto from = history[0].get_from();
        const auto count = history[0].get_count();
        const auto to = history[0].get_to();
        result += "[FROM:" + std::to_string(from) + ",COUNT:" + std::to_string(count) + ",TO:" + std::to_string(to) +
                "]    Previous >> Current\n";
        int current_max_hidden = 0;
        for (auto &item: hiddenCards) {
            if (!item.empty() && item.size() > current_max_hidden) {
                current_max_hidden = item.size();
            }
        }
        int previous_max_hidden = 0;
        for (auto &item: previous->hiddenCards) {
            if (!item.empty() && item.size() > previous_max_hidden) {
                previous_max_hidden = item.size();
            }
        }
        if (current_max_hidden > 0 || previous_max_hidden > 0) {
            const int max_hidden = std::max(current_max_hidden, previous_max_hidden);
            result += "\nHIDDEN CARDS:\n";
            const auto previousArray = Helper::split(previous->hidden_string(0, max_hidden), "\n");
            const auto currentArray = Helper::split(hidden_string(0, max_hidden), "\n");
            for (size_t i = 0; i < previousArray.size(); i++) {
                result += previousArray[i];
                result += " -> ";
                result += currentArray[i];
                result += "\n";
            }
        }

        int current_max_visible = 0;
        for (auto &item: visibleCards) {
            if (!item.empty() && item.size() > current_max_visible) {
                current_max_visible = item.size();
            }
        }
        int previous_max_visible = 0;
        for (auto &item: previous->visibleCards) {
            if (!item.empty() && item.size() > previous_max_visible) {
                previous_max_visible = item.size();
            }
        }
        if (current_max_visible > 0 || previous_max_visible > 0) {
            const int max_visible = std::max(current_max_visible, previous_max_visible);
            result += "\nVISIBLE CARDS:\n";
            const auto previousArray = Helper::split(previous->visible_string(0, max_visible), "\n");
            const auto currentArray = Helper::split(visible_string(0, max_visible), "\n");
            for (size_t i = 0; i < previousArray.size(); i++) {
                result += previousArray[i];
                result += " -> ";
                result += currentArray[i];
                result += "\n";
            }
        }

        if (deckCard.size() > 0 || previous->deckCard.size() > 0) {
            result += "\nDECK CARDS:\n";
            const int max = std::max(deckCard.size(), previous->deckCard.size());
            std::string previousStr;
            for (int i = 0; i < max; i++) {
                if (i % 10 == 0 && i != 0)
                    previousStr += "\n";
                previousStr += i < previous->deckCard.size() ? previous->deckCard[i]->to_string() : spd::EmptyCard;
            }
            std::string currentStr;
            for (int i = 0; i < max; i++) {
                if (i % 10 == 0 && i != 0)
                    currentStr += "\n";
                currentStr += i < deckCard.size() ? deckCard[i]->to_string() : spd::EmptyCard;
            }
            auto previousArray = Helper::split(previousStr, "\n");
            auto currentArray = Helper::split(currentStr, "\n");
            for (size_t i = 0; i < previousArray.size(); i++) {
                result += previousArray[i];
                result += " -> ";
                result += currentArray[i];
                result += "\n";
            }
        }
    } else {
        result += "\n";
        result += to_string();
    }
    return result;
}

bool State::detect_collection(const int index) {
    int set = 1;
    if (!visibleCards[index].empty()) {
        // # 该列存在可见牌
        int suit = visibleCards[index][0]->suit;
        for (auto card: visibleCards[index]) {
            if (card->value == set && suit == card->suit) {
                // # 同色才能收牌
                set++;
            } else {
                set = -1;
                break;
            }
        }
    }

    bool collection = false;
    if (set == 14) {
        // # 1-13全了,可以收一套牌
        collection = true;
        const size_t tmpCount = std::min<size_t>(13, visibleCards[index].size());
        visibleCards[index].erase(visibleCards[index].begin(), visibleCards[index].begin() + tmpCount);
        card_count -= 13;
        collection_steps.push_back(history.size() + 1);
        if (visibleCards[index].empty() && !hiddenCards[index].empty()) {
            // # 收牌后需要展示新的hidden牌的情况
            visibleCards[index].insert(visibleCards[index].begin(), std::make_move_iterator(hiddenCards[index].begin()),
                                       std::make_move_iterator(hiddenCards[index].begin() + 1));
            hiddenCards[index].erase(hiddenCards[index].begin(), hiddenCards[index].begin() + 1);
        }
    }
    return collection;
}

int State::flop_valuation(int limit, bool divide) const {
    if (previous == nullptr)
        // # 没有上一步
        return 0;
    const auto from = history[0].get_from();
    const auto count = history[0].get_count();
    const auto to = history[0].get_to();
    if (from < 0 || count < 0 || to < 0)
        // # 忽略发牌
        return 0;
    if (previous->visibleCards[from].size() == count && !previous->hiddenCards[from].empty()) {
        // # 可以翻出新牌 flop new card
        int depth = 0;
        return check_flop(this, from, depth, divide ? limit / 2 : limit);
    }
    return 0;
}

int State::check_flop(const State *state, int column, int &depth, const int limit) const {
    if (depth++ > limit)
        return 0;
    int result = 0;
    int value = state->visibleCards[column][0]->value;
    std::unordered_set<State *, StatePtrHash, StatePtrEqual> setTo;
    std::unordered_set<State *, StatePtrHash, StatePtrEqual> setCome;
    for (size_t i = 0; i < state->visibleCards.size(); i++) {
        if (i == column)
            continue;
        if (state->visibleCards[i].empty())
            // # 向空列移动不加分
            continue;
        if (state->visibleCards[i][0]->value == value + 1) {
            // # 可以向其他列移动
            State *newState = Solver::create_new_state(state, std::vector<Card *>{state->visibleCards[column][0]},
                                                       column, i);
            setTo.insert(newState);
            result += 2;
        }
        std::vector<Card *> moveList;
        for (size_t x = 0; x < state->visibleCards[i].size(); x++) {
            if (x == 0) {
                moveList.push_back(state->visibleCards[i][0]);
                continue;
            }
            auto cur = state->visibleCards[i][x];
            if (cur->suit == moveList.back()->suit && cur->value == moveList.back()->value + 1) {
                moveList.push_back(cur);
            } else
                break;
        }
        if (moveList.back()->value + 1 == value) {
            // # 可以移动到新翻开牌的位置
            auto newState = Solver::create_new_state(state, moveList, i, column);
            setCome.insert(newState);
            result += 1;
        }
    }
    // # 计算额外翻牌分
    for (auto &item: setCome) {
        result += item->flop_valuation(limit, true);
        delete item;
    }
    for (auto &item: setTo) {
        result += item->flop_valuation(limit, false);
        delete item;
    }
    return result;
}

int State::extra_valuation_more_suit() const {
    if (get_suit_count() <= 1)
        return 0;
    int result = blank_column_count() * 200; // # 空列加200分
    if (history.empty())
        return result;
    if (previous == nullptr)
        return result;
    const auto from = history[0].get_from();
    const auto count = history[0].get_count();
    const auto to = history[0].get_to();
    if (from < 0 || count < 0 || to < 0)
        return result;
    if (isblank(from))
        return result;
    if (previous->is_blank(to)) {
        // # 向空列移动
        for (size_t i = 0; i < previous->visibleCards.size(); i++) {
            if (previous->visibleCards[i].empty())
                continue;
            if (i == to)
                continue;
            std::vector<Card *> list;
            for (size_t x = 0; x < previous->visibleCards[i].size(); x++) {
                if (x == 0)
                    list.push_back(previous->visibleCards[i][0]);
                else {
                    if (previous->visibleCards[i][x]->suit == list.back()->suit && previous->visibleCards[i][x]->value -
                        1 == list.back()->value)
                        list.push_back(previous->visibleCards[i][x]);
                    else
                        break;
                }
            }
            if (visibleCards[from][0]->suit == list.back()->suit) {
                // # 同花色
                if (list.back()->value + 1 == visibleCards[from][0]->value)
                    result += 100;
                else {
                    std::vector<Card *> st;
                    for (size_t c = 0; c < visibleCards[from].size(); c++) {
                        if (c == 0)
                            st.push_back(visibleCards[from][0]);
                        else {
                            if (visibleCards[from][c]->suit == st.back()->suit && visibleCards[from][c]->value - 1 == st
                                .back()->value)
                                st.push_back(visibleCards[from][c]);
                            else
                                break;
                        }
                    }
                    if (list[0]->value - 1 == st.back()->value)
                        result += 100;
                }
            }
        }
    }
    return result;
}

int State::blank_column_count() const {
    int result = 0;
    for (size_t i = 0; i < visibleCards.size(); i++) {
        if (is_blank(i)) {
            result++;
        }
    }
    return result;
}

std::string State::hidden_string(const int row, const int max) const {
    if (max == 0)
        return "";
    if (row == max - 1)
        return floor_hidden_string(row);
    return floor_hidden_string(row) + "\n" + hidden_string(row + 1, max);
}

std::string State::floor_hidden_string(const int row) const {
    std::string result;
    for (auto &column: hiddenCards) {
        if (column.size() > row) {
            result += column[column.size() - row - 1]->to_string();
        } else {
            result += spd::EmptyCard;
        }
    }
    return result;
}

std::string State::visible_string(const int row, const int max) const {
    if (max == 0)
        return "";
    if (row == max - 1)
        return floor_visible_string(row);
    return floor_visible_string(row) + "\n" + visible_string(row + 1, max);
}

std::string State::floor_visible_string(const int row) const {
    std::string result;
    for (auto &column: visibleCards) {
        if (column.size() > row) {
            result += column[column.size() - row - 1]->to_string();
        } else {
            result += spd::EmptyCard;
        }
    }
    return result;
}

std::string State::deck_string() const {
    std::string result;
    for (size_t i = 0; i < deckCard.size(); i++) {
        if (i % 10 == 0 && i != 0) {
            result += "\n";
        }
        result += deckCard[i]->to_string();
    }
    return result;
}

size_t State::get_memory_usage() const {
    size_t total = 0;
    total += sizeof(*this);
    total += sizeof(poker);
    total += get_vector_memory(deckCard);
    total += get_nested_vector_memory(hiddenCards);
    total += get_nested_vector_memory(visibleCards);
    total += get_vector_memory(history);
    total += get_vector_memory(collection_steps);
    total += sizeof(previous);
    total += sizeof(columnValuation);
    return total;
}

std::string State::to_serialized() const {
    std::string result;
    for (size_t i = 0; i < 10; i++) {
        result += std::to_string(visibleCards[i].size());
        result += ",";
        for (const auto &card: visibleCards[i]) {
            result += card->to_char();
        }
        for (const auto &card: hiddenCards[i]) {
            result += card->to_char();
        }
        result += ";";
    }
    if (deckCard.empty()) {
        result += "*";
    } else {
        for (const auto &card: deckCard) {
            result += card->to_char();
        }
    }
    return result;
}

std::ostream &operator<<(std::ostream &out, const State &state) {
    out << state.to_full_string();
    return out;
}
