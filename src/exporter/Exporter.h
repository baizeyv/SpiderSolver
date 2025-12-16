//
// Created by baizeyv on 2025/5/1.
//

#ifndef EXPORTER_H
#define EXPORTER_H
#include "../State.h"
#include "../data/LevelData.h"


class Exporter {
    std::string full_file_path;
public:

    explicit Exporter(std::string  file_path);

    void export_csv(int id, const State& state, bool is_null = false) const;

    void export_csv(const LevelData& data) const;

    ~Exporter();
};



#endif //EXPORTER_H
