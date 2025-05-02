//
// Created by baizeyv on 2025/5/3.
//

#ifndef IMEMUSAGE_H
#define IMEMUSAGE_H

class IMemUsage
{
public:
    virtual ~IMemUsage() = default;
    virtual size_t get_memory_usage() const = 0;
};

#endif //IMEMUSAGE_H
