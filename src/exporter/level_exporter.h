//
// Created by baizeyv on 11/20/2025.
//

#ifndef SPIDERSOLVER_LEVEL_EXPORTER_H
#define SPIDERSOLVER_LEVEL_EXPORTER_H
#include <string>

#include "../State.h"

class level_exporter {
    /**
     * * 导出文件完整路径
     */
    std::string full_file_path;

public:
    explicit level_exporter(std::string file_path);
    void export_csv(int id, const State& state) const;
};


#endif //SPIDERSOLVER_LEVEL_EXPORTER_H