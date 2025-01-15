// src/calculations/strategies.cpp
#include "strategies.hpp"
#include <random>
#include <chrono>

namespace stock_analyzer {

double DefaultStrategy::project_change(const StockData& data, const std::string& start_date, int period) {
    // Very simple strategy: project the same percentage change as the previous period
    auto it = data.daily_prices.find(start_date);
    if (it == data.daily_prices.end()) {
        throw std::runtime_error("Date not found in stock data");
    }

    double start_price = it->second.close;
    double prev_price = start_price;

    // Find price from previous period
    for (int i = 0; i < period && it != data.daily_prices.begin(); ++i) {
        --it;
    }
    
    if (it != data.daily_prices.begin()) {
        prev_price = it->second.close;
    }

    // Calculate percentage change
    double prev_change = (start_price - prev_price) / prev_price;
    return prev_change * 100.0;
}

double RandomStrategy::project_change(const StockData& data, const std::string& start_date, int period) {
    // Random strategy: generate a random percentage change between -5% and 5%
    static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dis(-5.0, 5.0);
    
    return dis(gen);
}

}  // namespace stock_analyzer