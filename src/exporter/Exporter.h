//
// Created by baizeyv on 2025/5/1.
//

#ifndef EXPORTER_H
#define EXPORTER_H
#include "../State.h"


class Exporter {
private:
    std::string full_file_path;
public:
    explicit Exporter(std::string  file_path);
    ~Exporter();
    void export_csv(int id, const State& state, bool is_null = false) const;
};



#endif //EXPORTER_H
