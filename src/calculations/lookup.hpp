// src/calculations/lookup.hpp
#pragma once
#include "common/types.hpp"
#include <string>
#include <vector>

namespace stock_analyzer {

class StockLookup {
public:
    static std::vector<StockData> lookup_stocks(const std::vector<StockData>& data, const std::string& date);
};

}  // namespace stock_analyzer