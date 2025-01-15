// src/common/types.hpp
#pragma once
#include <string>
#include <vector>
#include <map>

namespace stock_analyzer {

struct StockPrice {
    std::string date;
    std::string sector;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

struct StockData {
    std::string symbol;
    std::string sector;
    std::map<std::string, StockPrice> daily_prices;
};

struct ProjectionResult {
    std::string ticker;
    double projected_change;
    double actual_change;
    double accuracy;
};

struct ProjectionSummary {
    double total_projected_change;
    double total_actual_change;
    double total_accuracy;
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

struct RebalanceAction {
    enum class Type {
        SELL,
        BUY
    };
    
    Type action_type;
    std::string ticker;
    int quantity;
    double projected_change;
    double actual_change;
    double projected_value;  // gains or losses
    double actual_value;    // gains or losses
    double accuracy;
};

}  // namespace stock_analyzer
