//
// Created by baizeyv on 5/8/2025.
//

#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <vector>

#include "Card.h"
#include "State.h"

using std::vector;

namespace spd {
    class calculator {
    public:
        /**
         * * 计算一列可见牌的估值
         * @param cards
         * @return
         */
        static double calculate_valuation_in_column(const vector<Card*>& cards);

        /**
         * * 计算一列中隐藏牌的惩罚分数
         * @param cards
         * @return
         */
        static double calculate_penalty_in_hidden_column(const vector<Card*>& cards);

        /**
         * * 检测是否可以间接收牌
         * @param visible_cards
         * @return
         */
        static bool detect_indirect_collection(const vector<vector<Card*>>& visible_cards);

        /**
         * * 寻找在一列中的可移动的部分
         * @param cards
         * @return
         */
        static vector<Card*> find_movable_cards_in_column(const vector<Card*> &cards);

        /**
         * * 判断未来是否可借助空列再次形成空列
         * @param state
         * @return
         */
        static bool detect_form_blank_with_help_of_blank(const State* state);

        /**
         * * 判断未来是否可以不借助空列形成新的空列
         * @param state
         * @return
         */
        static bool detect_form_blank_without_blank(const State* state);
    private:
        /**
         * * 序列中估值叠加
         * @param num
         * @param top_point
         * @param result
         */
        static void add_value(const int &num, const int &top_point, double &result);
    };

    // TODO:
    enum priority {
        after_deal = 1, // # 后发牌
        deal = 2, // # 发牌
        sequence_move = 3, // # 序列移动
        create_blank = 4, // # 未来可构建空列
        flop_new = 5, // # 可翻开新牌
        blank_without_blank = 6, // #未来不需要借助空白列就可以构建新的空白列
        blank_by_blank = 7, // # 未来可借助空列重新构建新空列
        blank = 8, // # 当前已构建出空列
        indirect_collection = 9, // # 可间接收牌
        collection = 10, // # 可收牌
    };

    // * 优先级单位
    inline constexpr int priority_unit = 1000000;
    // * 完成一套牌的奖励分数
    inline constexpr int finished_reward_score = 200;
    // * 一列空列的奖励分数
    inline constexpr int blank_column_reward_score = 100;
}



#endif //CALCULATOR_H
