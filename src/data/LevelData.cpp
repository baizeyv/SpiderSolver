//
// Created by baizeyv on 2025/5/1.
//

#include "LevelData.h"

#include <ranges>

#include "../Joker.h"

LevelData::LevelData() :
    id(0), seed(""), suit_count(-1), calc(-1), difficulty(-1), step1(-1), step2(-1), step3(-1), step4(-1), step5(-1),
    step6(-1), step7(-1), step8(-1), history(""), level(""), serialized(""), str(""), first_movable_count(-1),
    first_empty_column_count(-1) {}

LevelData::LevelData(const int id, const std::string &seed, const int suit_count, const int calc,
                     const float difficulty, const int step1, const int step2, const int step3, const int step4,
                     const int step5, const int step6, const int step7, const int step8, const std::string &history,
                     const std::string &level, const std::string &serialized, const std::string &str) :
    id(id), seed(seed), suit_count(suit_count), calc(calc), difficulty(difficulty), step1(step1), step2(step2),
    step3(step3), step4(step4), step5(step5), step6(step6), step7(step7), step8(step8), history(history), level(level),
    serialized(serialized), str(str), first_movable_count(-1), first_empty_column_count(-1) {}

LevelData::LevelData(const int id, const std::string &seed, const int suit_count, const int calc,
                     const float difficulty, const int step1, const int step2, const int step3, const int step4,
                     const int step5, const int step6, const int step7, const int step8, const std::string &history,
                     const std::string &level, const std::string &serialized, const std::string &str,
                     const int first_movable_count, const int first_empty_column_count) :
    id(id), seed(seed), suit_count(suit_count), calc(calc), difficulty(difficulty), step1(step1), step2(step2),
    step3(step3), step4(step4), step5(step5), step6(step6), step7(step7), step8(step8), history(history), level(level),
    serialized(serialized), str(str), first_movable_count(first_movable_count),
    first_empty_column_count(first_empty_column_count) {}

LevelData::LevelData(const int id, const State &state, const bool is_null) :
    id(id), first_movable_count(-1), first_empty_column_count(-1) {
    seed = state.poker->mark;
    suit_count = state.poker->suitCount;
    calc = state.calc;
    if (is_null) {
        step1 = step2 = step3 = step4 = step5 = step6 = step7 = step8 = -1;
        difficulty = -1;
        history = "[UNSOLVED]";
    } else {
        step1 = state.collection_steps[0];
        step2 = state.collection_steps[1];
        step3 = state.collection_steps[2];
        step4 = state.collection_steps[3];
        step5 = state.collection_steps[4];
        step6 = state.collection_steps[5];
        step7 = state.collection_steps[6];
        step8 = state.collection_steps[7];
        difficulty = -100000.0 / calc + 1000.0;
        for (auto &item: std::ranges::reverse_view(state.history)) {
            const auto from = item.get_from();
            const auto to = item.get_to();
            const auto count = item.get_count();
            const std::string his =
                    "[F:" + std::to_string(from) + ",T:" + std::to_string(to) + ",N:" + std::to_string(count) + "]>";
            history += his;
        }
    }
    level = state.poker->get_level();
    serialized = state.poker->to_serialized();
    str = state.poker->to_level_string();
    auto joker = Joker(serialized);
    first_movable_count = joker.get_first_movable_count();
    first_empty_column_count = Joker::get_first_empty_column_count(history);
}

LevelData::~LevelData() = default;

std::ostream &operator<<(std::ostream &os, const LevelData &data) {
    const std::string split(",");
    const std::string quotation("\"");
    os << quotation << std::to_string(data.id) << quotation << split << quotation << data.seed << quotation << split
       << quotation << std::to_string(data.calc) << quotation << split << quotation << std::to_string(data.difficulty)
       << quotation << split << quotation << std::to_string(data.step1) << quotation << split << quotation
       << std::to_string(data.step2) << quotation << split << quotation << std::to_string(data.step3) << quotation
       << split << quotation << std::to_string(data.step4) << quotation << split << quotation
       << std::to_string(data.step5) << quotation << split << quotation << std::to_string(data.step6) << quotation
       << split << quotation << std::to_string(data.step7) << quotation << split << quotation
       << std::to_string(data.step8) << quotation << split << quotation << std::to_string(data.suit_count) << quotation
       << split << quotation << data.history << quotation << split << quotation << data.level << quotation << split
       << quotation << data.serialized << quotation << split << quotation << data.str << quotation << split << quotation
       << std::to_string(data.first_movable_count) << quotation << split << quotation
       << std::to_string(data.first_empty_column_count) << quotation;

    return os;
}
