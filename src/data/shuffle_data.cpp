//
// Created by baizeyv on 5/23/2025.
//

#include "shuffle_data.h"

shuffle_data::shuffle_data(const int id, const State &state) : id(id) {
    suit_count = state.poker->suitCount;
    level = state.to_level();
    serialized = state.to_serialized();
}

std::ostream & operator<<(std::ostream &os, const shuffle_data &data) {
    const std::string split(",");
    const std::string quotation("\"");
    os << quotation << std::to_string(data.id) << quotation << split
        << quotation << std::to_string(data.suit_count) << quotation << split
        << quotation << data.level << quotation << split
        << quotation << data.serialized << quotation;
    return os;
}
