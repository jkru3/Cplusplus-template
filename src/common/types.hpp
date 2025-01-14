// src/common/types.hpp
#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

namespace stock_analyzer {

struct StockPrice {
    std::string date;
    std::string sector;
    double open;
    double high;
    double low;
    double close;
    int volume;
};

struct StockData {
    std::string symbol;
    std::string sector;
    std::map<std::string, StockPrice> daily_prices;  // date -> price data
};

struct PortfolioHolding {
    std::string ticker;
    int quantity;
};

struct Portfolio {
    std::string id;
    std::string date;
    double cash;
    std::vector<PortfolioHolding> holdings;
};

struct PredictionResult {
    std::string portfolio_id;
    std::map<std::string, double> predicted_prices;
    double predicted_return;
    double actual_return;
    double accuracy_score;
};

// JSON serialization declarations
void to_json(nlohmann::json& j, const StockPrice& p);
void from_json(const nlohmann::json& j, StockPrice& p);
void to_json(nlohmann::json& j, const StockData& s);
void from_json(const nlohmann::json& j, StockData& s);
void to_json(nlohmann::json& j, const PortfolioHolding& h);
void from_json(const nlohmann::json& j, PortfolioHolding& h);
void to_json(nlohmann::json& j, const Portfolio& p);
void from_json(const nlohmann::json& j, Portfolio& p);

}  // namespace stock_analyzer