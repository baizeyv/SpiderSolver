//
// Created by baizeyv on 11/20/2025.
//

#include "level_data.h"

level_data::level_data(const int id, const State &state) : id(id) {
    suit_count = state.poker->suitCount;
    level_num = state.to_level();
    level_str = state.to_level_str();
}

std::ostream & operator<<(std::ostream &os, const level_data &data) {
    const std::string split(",");
    const std::string quotation("\"");
    os << quotation << std::to_string(data.id) << quotation << split
        << quotation << std::to_string(data.suit_count) << quotation << split
        << quotation << data.level_num << quotation << split
        << quotation << data.level_str << quotation;
    return os;
}
