// src/main.cpp
#include "data_processing/csv_parser.hpp"
#include "data_processing/json_parser.hpp"
#include "calculations/lookup.hpp"
#include "calculations/speculation.hpp"
#include "calculations/format.hpp"
#include "calculations/strategies.hpp"
#include "calculations/rebalance.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
#include <memory>
#include <chrono>

using namespace stock_analyzer;

std::string get_current_date() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
    return std::string(buffer);
}

// Helper function to safely check if we have enough future data points
bool has_enough_future_data(const std::map<std::string, StockPrice>::const_iterator& start_it,
                          const std::map<std::string, StockPrice>& prices,
                          int period) {
    auto temp_it = start_it;
    for (int i = 0; i < period; ++i) {
        if (std::next(temp_it) == prices.end()) {
            return false;
        }
        temp_it = std::next(temp_it);
    }
    return true;
}

int main(int argc, char* argv[]) {
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_level(spdlog::level::debug);

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [lookup|project|rebalance] <csv_file> [date] [period] [strategy]\n";
        return 1;
    }

    std::string command = argv[1];
    std::string csv_file = argv[2];
    std::string date = argc > 3 ? argv[3] : get_current_date();

    try {
        if (!std::filesystem::exists(csv_file)) {
            throw std::runtime_error("File does not exist: " + csv_file);
        }

        if (!std::filesystem::is_regular_file(csv_file)) {
            throw std::runtime_error("Path is not a regular file: " + csv_file);
        }

        auto stock_data = CSVParser::parse_stock_data(csv_file);

        if (command == "lookup") {
            std::cout << "\nStocks for " << date << ":\n";
            std::cout << "Ticker | Sector             | Open     | High     | Low      | Close    | Volume\n";
            std::cout << "--------------------------------------------------------------------------------\n";
            auto results = StockLookup::lookup_stocks(stock_data, date);
            for (const auto& stock : results) {
                std::cout << Format::format_lookup_result(stock, date) << std::endl;
            }
            std::cout << "\n";
        }
        else if (command == "project") {
            int period = argc > 4 ? std::stoi(argv[4]) : 7;
            std::string strategy_name = argc > 5 ? argv[5] : "default";

            std::shared_ptr<Strategy> strategy;
            if (strategy_name == "random") {
                strategy = std::make_shared<RandomStrategy>();
            } else {
                strategy = std::make_shared<DefaultStrategy>();
            }

            auto [results, summary] = Speculation::project_stocks(stock_data, date, period, strategy);
            
            // Determine if we should include actual results
            bool include_actual = false;
            for (const auto& stock : stock_data) {
                auto it = stock.daily_prices.find(date);
                if (it != stock.daily_prices.end() && has_enough_future_data(it, stock.daily_prices, period)) {
                    include_actual = true;
                    break;
                }
            }

            // Print results
            std::cout << "\nSpeculation for " << date << " " << period << " trading days ahead using " << strategy_name << " strategy:\n";
            std::cout << Format::format_projection_header(include_actual);
            for (const auto& result : results) {
                std::cout << Format::format_projection_result(result, include_actual) << std::endl;
            }
            std::cout << "Total: | " << Format::format_projection_summary(summary, include_actual) << std::endl;
            std::cout << "\n";
        } else if (command == "rebalance" && argc > 3) {
            // Parse portfolio file
            std::string portfolio_file = argv[3];
            Portfolio portfolio = JSONParser::parse_portfolio_json(portfolio_file);
            
            int period = argc > 4 ? std::stoi(argv[4]) : 5;
            int max_holdings = argc > 5 ? std::stoi(argv[5]) : 3;
            std::string strategy_name = argc > 6 ? argv[6] : "default";
            
            std::shared_ptr<Strategy> strategy;
            if (strategy_name == "random") {
                strategy = std::make_shared<RandomStrategy>();
            } else {
                strategy = std::make_shared<DefaultStrategy>();
            }
            
            auto actions = PortfolioRebalancer::rebalance_portfolio(
                portfolio, stock_data, period, max_holdings, strategy);
            
            // Determine if we should include actual results
            bool include_actual = false;
            for (const auto& stock : stock_data) {
                auto it = stock.daily_prices.find(portfolio.date);
                if (it != stock.daily_prices.end() && has_enough_future_data(it, stock.daily_prices, period)) {
                    include_actual = true;
                    break;
                }
            }
            
            // Print results
            for (const auto& action : actions) {
                std::cout << Format::format_rebalance_action(action, include_actual) << std::endl;
            }
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}