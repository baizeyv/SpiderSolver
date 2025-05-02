//
// Created by baizeyv on 2025/5/3.
//

#include "HistoryItem.h"

int8_t HistoryItem::get_from() const
{
    return decode_field((packed >> 0) & 0xF, 9);
}

int8_t HistoryItem::get_to() const
{
    return decode_field((packed >> 4) & 0xF, 9);
}

int8_t HistoryItem::get_count() const
{
    return decode_field((packed >> 8) & 0xF, 12);
}

bool HistoryItem::get_collection() const
{
    return (packed >> 12) & 0x1;
}

void HistoryItem::set_from(const int8_t val)
{
    packed = (packed & ~(0xF << 0)) | (encode_field(val, 9) << 0);
}

void HistoryItem::set_to(const int8_t val)
{
    packed = (packed & ~(0xF << 4)) | (encode_field(val, 9) << 4);
}

void HistoryItem::set_count(const int8_t val)
{
    packed = (packed & ~(0xF << 8)) | (encode_field(val, 12) << 8);
}

void HistoryItem::set_collection(const bool val)
{
    packed = (packed & ~(1 << 12)) | ((val ? 1 : 0) << 12);
}

uint8_t HistoryItem::encode_field(const int8_t value, const int8_t max)
{
    if (value < -1 || value > max)
        return 0xF; // # invalid marker
    return static_cast<uint8_t>(value + 1);
}

int8_t HistoryItem::decode_field(const uint8_t raw, const int8_t max)
{
    if (raw == 0xF)
        return -127; // # special invalid marker
    const int8_t value = static_cast<int8_t>(raw) - 1;
    if (value > max)
        return -127;
    return value;
}
