//
// Created by baizeyv on 2025/5/1.
//

#include "Exporter.h"

#include <filesystem>
#include <sstream>
#include <fstream>
#include <utility>

#include "../Helper.h"
#include "../data/LevelData.h"

Exporter::Exporter(std::string file_path) : full_file_path(std::move(file_path))
{
}

Exporter::~Exporter() = default;

void Exporter::export_csv(const int id, const State& state, bool is_null) const
{
    const LevelData data(id, state, is_null);
    std::ostringstream oss;
    oss << data;
    auto content = oss.str();
    Helper::check_file_and_create_dir_when_needed(full_file_path);
    if (!std::filesystem::exists(full_file_path))
    {
        std::ofstream writer(full_file_path, std::ios::app);
        writer << "id,seed,calc,difficulty,step1,step2,step3,step4,step5,step6,step7,step8,suitCount,history,level,serialized\n";
        writer << content << "\n";
    }
    else
    {
        std::ofstream writer(full_file_path, std::ios::app);
        writer << content << "\n";
    }
}
