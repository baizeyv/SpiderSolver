//
// Created by baizeyv on 5/12/2025.
//

#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <map>
#include <vector>

#include "csv_data.h"
#include "../data/LevelData.h"


class csv_parser {
public:
    std::map<std::string, std::vector<LevelData>> data_map;

    /**
     * * 注入文件
     * @param filename
     */
    void inject(const std::string& filename);

    /**
     * * 筛选LevelData
     */
    void filter();

    /**
     * * 解析文件
     * @param filename
     * @return
     */
    static std::vector<csv_data> parse(const std::string& filename);

    /**
     * * 读取一行csv将其转为csv_data
     * @param line
     * @return
     */
    static csv_data parse(std::string& line);

    /**
     * * 将csv_data转为LevelData
     * @param data
     * @return
     */
    static LevelData parse(const csv_data& data);

    static std::vector<LevelData> convert(const std::vector<csv_data>& data);
};



#endif //CSV_PARSER_H
