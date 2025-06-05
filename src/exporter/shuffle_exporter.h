//
// Created by baizeyv on 5/23/2025.
//

#ifndef SHUFFLE_EXPORTER_H
#define SHUFFLE_EXPORTER_H
#include <string>
#include "../State.h"


class shuffle_exporter {
    /**
     * * 导出文件完整路径
     */
    std::string full_file_path;

public:
    explicit shuffle_exporter(std::string file_path);
    void export_csv(int id, const State& state) const ;
};



#endif //SHUFFLE_EXPORTER_H
