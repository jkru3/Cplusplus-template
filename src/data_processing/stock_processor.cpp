// src/data_processing/stock_processor.cpp
#include "data_processing/stock_processor.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace stock_analyzer {

StockProcessor::StockProcessor() {}

std::vector<StockData> StockProcessor::process_stocks(const std::string& input_dir) {
    std::vector<StockData> processed_data;
    
    for (const auto& entry : std::filesystem::directory_iterator(input_dir)) {
        if (entry.path().extension() == ".csv") {
            spdlog::debug("Processing file: {}", entry.path().string());
            processed_data.push_back(process_single_stock(entry.path().string()));
        }
    }
    
    return processed_data;
}

StockData StockProcessor::process_single_stock(const std::string& file_path) {
    StockData data;
    // Implementation: Read CSV file and populate StockData
    // Calculate metrics like average price, D/E ratio, etc.
    return data;
}

void StockProcessor::save_processed_data(const std::vector<StockData>& data, 
                                       const std::string& output_path) {
    nlohmann::json j = data;
    std::ofstream o(output_path);
    o << j.dump(4);
}

std::vector<StockData> StockProcessor::load_processed_data(const std::string& input_path) {
    std::ifstream i(input_path);
    nlohmann::json j;
    i >> j;
    return j.get<std::vector<StockData>>();
}

}  // namespace stock_analyzer