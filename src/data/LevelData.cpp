//
// Created by baizeyv on 2025/5/1.
//

#include "LevelData.h"

#include <ranges>

LevelData::LevelData(const int id, const State& state, bool is_null) : id(id)
{
    seed = state.poker->mark;
    suit_count = state.poker->suitCount;
    calc = state.calc;
    if (is_null)
    {
        step1 =
            step2 =
            step3 =
            step4 =
            step5 =
            step6 =
            step7 =
            step8 = -1;
        difficulty = -1;
        history = "[UNSOLVED]";
    }
    else
    {
        step1 = state.collection_steps[0];
        step2 = state.collection_steps[1];
        step3 = state.collection_steps[2];
        step4 = state.collection_steps[3];
        step5 = state.collection_steps[4];
        step6 = state.collection_steps[5];
        step7 = state.collection_steps[6];
        step8 = state.collection_steps[7];
        difficulty = static_cast<float>(-100000) / static_cast<float>(calc) + 1000;
        for (auto [from, count, to, collection] : std::ranges::reverse_view(state.history))
        {
            const std::string his = "[F:" + std::to_string(from) + ",T:" + std::to_string(to) + ",N:" + std::to_string(count) + "]>";
            history += his;
        }
    }
}

LevelData::~LevelData() = default;

std::ostream& operator<<(std::ostream& os, const LevelData& data)
{
    const std::string split(",");
    os << std::to_string(data.id) << split
        << data.seed << split << std::to_string(data.calc) << split
        << std::to_string(data.difficulty) << split
        << std::to_string(data.step1) << split
        << std::to_string(data.step2) << split
        << std::to_string(data.step3) << split
        << std::to_string(data.step4) << split
        << std::to_string(data.step5) << split
        << std::to_string(data.step6) << split
        << std::to_string(data.step7) << split
        << std::to_string(data.step8) << split
        << std::to_string(data.suit_count) << split
        << data.history;
    return os;
}
