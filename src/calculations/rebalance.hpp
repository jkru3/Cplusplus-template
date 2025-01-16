// src/calculations/rebalance.hpp
#pragma once
#include "common/types.hpp"
#include "strategies.hpp"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>

namespace stock_analyzer {

class PortfolioRebalancer {
public:
    static std::vector<RebalanceAction> 
    rebalance_portfolio(const Portfolio& portfolio,
                       const std::vector<StockData>& stock_data,
                       int period,
                       int max_holdings,
                       std::shared_ptr<Strategy> strategy);

private:
    static double get_stock_price(const StockData& stock, const std::string& date);
    static std::vector<std::pair<std::string, double>> 
    rank_stocks(const std::vector<StockData>& stock_data,
                const std::string& date,
                int period,
                std::shared_ptr<Strategy> strategy);
};

}  // namespace stock_analyzer