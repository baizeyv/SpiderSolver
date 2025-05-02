//
// Created by baizeyv on 2025/5/3.
//

#ifndef HISTORYITEM_H
#define HISTORYITEM_H
#include <cstdint>

struct HistoryItem
{
    uint16_t packed = 0;
    // getters
    int8_t get_from() const;
    int8_t get_to() const;
    int8_t get_count() const;
    bool get_collection() const;
    // setters
    void set_from(int8_t val);
    void set_to(int8_t val);
    void set_count(int8_t val);
    void set_collection(bool val);

private:
    static uint8_t encode_field(int8_t value, int8_t max);
    static int8_t decode_field(uint8_t raw, int8_t max);
};

#endif //HISTORYITEM_H
