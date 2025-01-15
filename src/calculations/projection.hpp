// src/calculations/projection.hpp
#pragma once
#include "common/types.hpp"
#include "strategies.hpp"
#include <string>
#include <vector>
#include <memory>

namespace stock_analyzer {

class StockProjection {
public:
    static std::pair<std::vector<ProjectionResult>, ProjectionSummary> 
    project_stocks(const std::vector<StockData>& data, 
                  const std::string& date,
                  int period,
                  std::shared_ptr<ProjectionStrategy> strategy);
};

class PortfolioRebalancer {
public:
    static std::vector<RebalanceAction> 
    rebalance_portfolio(const Portfolio& portfolio,
                       const std::vector<StockData>& stock_data,
                       int period,
                       int max_holdings,
                       std::shared_ptr<ProjectionStrategy> strategy);

private:
    static double get_stock_price(const StockData& stock, const std::string& date);
    static std::vector<std::pair<std::string, double>> 
    rank_stocks(const std::vector<StockData>& stock_data,
                const std::string& date,
                int period,
                std::shared_ptr<ProjectionStrategy> strategy);
};

}  // namespace stock_analyzer