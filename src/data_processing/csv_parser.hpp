// src/data_processing/csv_parser.hpp
#pragma once
#include "common/types.hpp"
#include <string>
#include <vector>

namespace stock_analyzer {

class CSVParser {
public:
    static std::vector<StockData> parse_stock_data(const std::string& filename);
    static std::vector<std::string> split(const std::string& str, char delimiter);
};

}  // namespace stock_analyzer