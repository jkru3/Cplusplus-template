// src/common/types.hpp
#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

namespace stock_analyzer {

struct StockData {
    std::string symbol;
    std::map<std::string, double> daily_prices;  // date -> price
    double debt_equity_ratio;
    double market_cap;
    // Add other financial metrics as needed
};

struct Portfolio {
    std::string id;
    std::map<std::string, double> holdings;  // symbol -> weight
};

struct PredictionResult {
    std::string portfolio_id;
    std::map<std::string, double> predicted_prices;
    double predicted_return;
    double actual_return;
    double accuracy_score;
};

// JSON serialization
void to_json(nlohmann::json& j, const StockData& s);
void from_json(const nlohmann::json& j, StockData& s);
void to_json(nlohmann::json& j, const Portfolio& p);
void from_json(const nlohmann::json& j, Portfolio& p);
}  // namespace stock_analyzer

// Add these outside the stock_analyzer namespace
namespace nlohmann {
    template <>
    struct adl_serializer<stock_analyzer::StockData> {
        static void to_json(json& j, const stock_analyzer::StockData& s) {
            stock_analyzer::to_json(j, s);
        }
        
        static void from_json(const json& j, stock_analyzer::StockData& s) {
            stock_analyzer::from_json(j, s);
        }
    };

    template <>
    struct adl_serializer<stock_analyzer::Portfolio> {
        static void to_json(json& j, const stock_analyzer::Portfolio& p) {
            stock_analyzer::to_json(j, p);
        }
        
        static void from_json(const json& j, stock_analyzer::Portfolio& p) {
            stock_analyzer::from_json(j, p);
        }
    };
}