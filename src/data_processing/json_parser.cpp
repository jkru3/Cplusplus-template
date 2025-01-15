// src/data_processing/json_parser.cpp
#include "json_parser.hpp"
#include <fstream>
#include <stdexcept>

namespace stock_analyzer {

Portfolio JSONParser::parse_portfolio_json(const std::string& filename) {
    // Read the JSON file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    try {
        // Parse JSON
        nlohmann::json json_data;
        file >> json_data;

        // Create Portfolio object
        Portfolio portfolio;
        
        // Parse basic fields
        portfolio.id = json_data["id"].get<std::string>();
        portfolio.date = json_data["date"].get<std::string>();
        portfolio.cash = json_data["cash"].get<double>();

        // Parse holdings array
        const auto& holdings = json_data["holdings"];
        for (const auto& holding : holdings) {
            PortfolioHolding h;
            h.ticker = holding["ticker"].get<std::string>();
            h.quantity = holding["quantity"].get<int>();
            portfolio.holdings.push_back(h);
        }

        return portfolio;
    }
    catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}

}  // namespace stock_analyzer