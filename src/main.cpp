// src/main.cpp
#include "data_processing/stock_processor.hpp"
#include "data_processing/portfolio_generator.hpp"
#include "prediction/predictor.hpp"
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
    CLI::App app{"Stock Analysis Tool"};

    // Command line parameters
    std::string input_dir;
    std::string output_dir;
    int portfolio_count = 0;
    bool process_stocks = false;
    bool generate_portfolios = false;
    bool run_prediction = false;

    // Set up CLI options
    app.add_flag("--process-stocks", process_stocks, "Process stock data");
    app.add_flag("--generate-portfolios", generate_portfolios, "Generate portfolios");
    app.add_flag("--predict", run_prediction, "Run prediction");
    app.add_option("--input", input_dir, "Input directory")->required();
    app.add_option("--output", output_dir, "Output directory")->required();
    app.add_option("--portfolio-count", portfolio_count, "Number of portfolios to generate");

    CLI11_PARSE(app, argc, argv);

    try {
        if (process_stocks) {
            spdlog::info("Processing stock data...");
            stock_analyzer::StockProcessor processor;
            auto processed_data = processor.process_stocks(input_dir);
            processor.save_processed_data(processed_data, output_dir + "/processed_stocks.json");
        }

        if (generate_portfolios) {
            spdlog::info("Generating portfolios...");
            stock_analyzer::StockProcessor processor;
            auto stock_data = processor.load_processed_data(input_dir + "/processed_stocks.json");
            
            std::vector<std::string> symbols;
            for (const auto& stock : stock_data) {
                symbols.push_back(stock.symbol);
            }
            
            stock_analyzer::PortfolioGenerator generator(symbols);
            auto portfolios = generator.generate_portfolios(portfolio_count);
            generator.save_portfolios(portfolios, output_dir + "/portfolios.json");
        }

        if (run_prediction) {
            spdlog::info("Running predictions...");
            stock_analyzer::StockProcessor processor;
            auto stock_data = processor.load_processed_data(input_dir + "/processed_stocks.json");
            
            // Load portfolios and run predictions
            // Implementation details...
        }
    }
    catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
        return 1;
    }

    return 0;
}
