// src/data_processing/stock_processor.hpp
#pragma once
#include "common/types.hpp"
#include <string>
#include <vector>

namespace stock_analyzer {

class StockProcessor {
public:
    StockProcessor();
    std::vector<StockData> process_stocks(const std::string& input_dir);
    void save_processed_data(const std::vector<StockData>& data, 
                           const std::string& output_path);
    std::vector<StockData> load_processed_data(const std::string& input_path);

private:
    StockData process_single_stock(const std::string& file_path);
    double calculate_metrics(const StockData& data);
};

}  // namespace stock_analyzer