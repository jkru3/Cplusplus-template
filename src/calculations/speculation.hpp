// src/calculations/speculation.hpp
#pragma once
#include "common/types.hpp"
#include "strategies.hpp"
#include <string>
#include <vector>
#include <memory>

namespace stock_analyzer {

class Speculation {
public:
    static std::pair<std::vector<SpeculationResult>, SpeculationSummary> 
    project_stocks(const std::vector<StockData>& data, 
                  const std::string& date,
                  int period,
                  std::shared_ptr<Strategy> strategy);
};

}  // namespace stock_analyzer