// src/calculations/lookup.cpp
#include "lookup.hpp"
#include <stdexcept>

namespace stock_analyzer {

std::vector<StockData> StockLookup::lookup_stocks(const std::vector<StockData>& data, const std::string& date) {
    std::vector<StockData> result;
    bool found_any = false;

    for (const auto& stock : data) {
        auto it = stock.daily_prices.find(date);
        if (it != stock.daily_prices.end()) {
            found_any = true;
            result.push_back(stock);
        }
    }

    if (!found_any) {
        throw std::runtime_error("date out of bounds");
    }

    return result;
};

}  // namespace stock_analyzer