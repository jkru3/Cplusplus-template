// src/data_processing/csv_parser.cpp
#include "data_processing/csv_parser.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <spdlog/spdlog.h>

namespace stock_analyzer {

std::vector<std::string> CSVParser::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(str);
    while (std::getline(token_stream, token, delimiter)) {
        // Trim whitespace
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<StockData> CSVParser::parse_stock_data(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    std::getline(file, line); // Skip header

    // Use map to group data by symbol
    std::map<std::string, StockData> stock_map;

    while (std::getline(file, line)) {
        auto fields = split(line, ',');
        if (fields.size() < 8) {
            spdlog::warn("Skipping malformed line: {}", line);
            continue;
        }

        std::string symbol = fields[0];
        
        StockPrice price;
        price.date = fields[1];
        price.sector = fields[2];
        price.open = std::stod(fields[3]);
        price.high = std::stod(fields[4]);
        price.low = std::stod(fields[5]);
        price.close = std::stod(fields[6]);
        price.volume = std::stoi(fields[7]);

        // Create or update stock data
        if (stock_map.find(symbol) == stock_map.end()) {
            stock_map[symbol].symbol = symbol;
            stock_map[symbol].sector = price.sector;
        }
        stock_map[symbol].daily_prices[price.date] = price;
    }

    // Convert map to vector
    std::vector<StockData> result;
    for (const auto& [symbol, data] : stock_map) {
        result.push_back(data);
    }

    return result;
}

}  // namespace stock_analyzer