// src/main.cpp
#include "common/types.hpp"
#include "data_processing/csv_parser.hpp"
#include "prediction/predictor.hpp"
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

// Helper function to load portfolio from JSON file
stock_analyzer::Portfolio load_portfolio(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open portfolio file: " + filename);
    }
    
    json j;
    file >> j;
    return j.get<stock_analyzer::Portfolio>();
}

int main(int argc, char** argv) {
    CLI::App app{"Stock Analysis Tool"};

    std::string stock_file;
    std::string portfolio_file;
    double max_investment = 0.0;
    
    app.add_option("--stocks", stock_file, "Stock data CSV file")->required();
    app.add_option("--portfolio", portfolio_file, "Portfolio JSON file")->required();
    app.add_option("--max-investment", max_investment, "Maximum investment amount (0 for current portfolio value)");

    CLI11_PARSE(app, argc, argv);

    try {
        // Load stock data
        spdlog::info("Loading stock data from {}", stock_file);
        auto stock_data = stock_analyzer::CSVParser::parse_stock_data(stock_file);
        spdlog::info("Loaded {} stocks", stock_data.size());
        
        // Load portfolio
        spdlog::info("Loading portfolio from {}", portfolio_file);
        auto portfolio = load_portfolio(portfolio_file);
        
        // Initialize predictor and get recommendations
        stock_analyzer::Predictor predictor(stock_data);
        auto recommendations = predictor.recommend_portfolio_adjustments(portfolio, max_investment);
        
        // Print results
        std::cout << "\nPortfolio Recommendations:\n";
        std::cout << "========================\n\n";
        
        // Print current portfolio value
        double current_value = portfolio.cash;
        std::cout << "Current Portfolio:\n";
        std::cout << "  Cash: $" << std::fixed << std::setprecision(2) << portfolio.cash << "\n";
        for (const auto& holding : portfolio.holdings) {
            std::cout << "  " << holding.ticker << ": " << holding.quantity << " shares\n";
        }
        std::cout << "\n";
        
        // Print recommendations
        std::cout << "Recommended Actions:\n";
        for (const auto& rec : recommendations) {
            std::cout << rec.ticker << ":\n";
            std::cout << "  Current Position: " << rec.current_quantity << " shares\n";
            std::cout << "  Recommended Position: " << rec.recommended_quantity << " shares\n";
            std::cout << "  Action: " << rec.action;
            if (rec.action != "HOLD") {
                int diff = std::abs(rec.recommended_quantity - rec.current_quantity);
                std::cout << " " << diff << " shares";
            }
            std::cout << "\n";
            std::cout << "  Expected Return: " << std::fixed << std::setprecision(2) 
                      << rec.predicted_return << "%\n\n";
        }
        
    } catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
        return 1;
    }

    return 0;
}