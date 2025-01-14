// src/data_processing/portfolio_generator.hpp
#pragma once
#include "common/types.hpp"

namespace stock_analyzer {

class PortfolioGenerator {
public:
    PortfolioGenerator(const std::vector<std::string>& available_symbols);
    std::vector<Portfolio> generate_portfolios(int count, 
                                             int min_stocks = 5, 
                                             int max_stocks = 20);
    void save_portfolios(const std::vector<Portfolio>& portfolios, 
                        const std::string& output_path);

private:
    std::vector<std::string> symbols_;
    Portfolio generate_single_portfolio(int num_stocks);
};

}  // namespace stock_analyzer