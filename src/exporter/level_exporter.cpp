//
// Created by baizeyv on 11/20/2025.
//

#include "level_exporter.h"

#include "../data/level_data.h"
#include <filesystem>
#include <sstream>
#include <fstream>

#include "../Helper.h"

level_exporter::level_exporter(std::string file_path) : full_file_path(std::move(file_path)) {
}

void level_exporter::export_csv(int id, const State &state) const {
    const level_data data(id, state);
    std::ostringstream oss;
    oss << data;
    auto content = oss.str();
    Helper::check_file_and_create_dir_when_needed(full_file_path);
    if (!std::filesystem::exists(full_file_path))
    {
        std::ofstream writer(full_file_path, std::ios::app);
        writer << "id,suit_count,level_num,level_str\n";
        writer << content << "\n";
    }
    else
    {
        std::ofstream writer(full_file_path, std::ios::app);
        writer << content << "\n";
    }
}

