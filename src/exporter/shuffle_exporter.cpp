//
// Created by baizeyv on 5/23/2025.
//

#include "shuffle_exporter.h"

#include "../data/shuffle_data.h"
#include <filesystem>
#include <sstream>
#include <fstream>

#include "../Helper.h"


shuffle_exporter::shuffle_exporter(std::string file_path) : full_file_path(std::move(file_path)) {
}

void shuffle_exporter::export_csv(int id, const State &state) const {
    const shuffle_data data(id, state);
    std::ostringstream oss;
    oss << data;
    auto content = oss.str();
    Helper::check_file_and_create_dir_when_needed(full_file_path);
    if (!std::filesystem::exists(full_file_path))
    {
        std::ofstream writer(full_file_path, std::ios::app);
        writer << "id,suit_count,level,serialized\n";
        writer << content << "\n";
    }
    else
    {
        std::ofstream writer(full_file_path, std::ios::app);
        writer << content << "\n";
    }
}
