// src/data_processing/portfolio_generator.cpp
#include "data_processing/portfolio_generator.hpp"
#include <random>
#include <fstream>

namespace stock_analyzer {

PortfolioGenerator::PortfolioGenerator(const std::vector<std::string>& available_symbols)
    : symbols_(available_symbols) {}

std::vector<Portfolio> PortfolioGenerator::generate_portfolios(int count, 
                                                             int min_stocks, 
                                                             int max_stocks) {
    std::vector<Portfolio> portfolios;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> stocks_dist(min_stocks, max_stocks);

    for (int i = 0; i < count; ++i) {
        int num_stocks = stocks_dist(gen);
        portfolios.push_back(generate_single_portfolio(num_stocks));
    }

    return portfolios;
}

Portfolio PortfolioGenerator::generate_single_portfolio(int num_stocks) {
    Portfolio portfolio;
    portfolio.id = "PF" + std::to_string(std::random_device{}());
    
    // Randomly select stocks and assign weights
    // Implementation details...

    return portfolio;
}

void PortfolioGenerator::save_portfolios(const std::vector<Portfolio>& portfolios, 
                                       const std::string& output_path) {
    nlohmann::json j = portfolios;
    std::ofstream o(output_path);
    o << j.dump(4);
}

}  // namespace stock_analyzer