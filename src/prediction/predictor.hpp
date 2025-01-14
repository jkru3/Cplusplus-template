// src/prediction/predictor.hpp
#pragma once
#include "common/types.hpp"
#include <string>
#include <vector>
#include <map>
#include <optional>

namespace stock_analyzer {

struct StockPrediction {
    std::string ticker;
    std::string sector;
    double current_price;
    double predicted_price;
    double predicted_return;
    double confidence_score;
};

struct PortfolioRecommendation {
    std::string ticker;
    int current_quantity;
    int recommended_quantity;
    std::string action;  // "BUY", "SELL", or "HOLD"
    double predicted_return;
};

class Predictor {
public:
    Predictor(const std::vector<StockData>& historical_data);
    
    // Analyze all stocks and return top predictions
    std::vector<StockPrediction> analyze_all_stocks(const std::string& date);
    
    // Generate portfolio recommendations
    std::vector<PortfolioRecommendation> recommend_portfolio_adjustments(
        const Portfolio& portfolio,
        double max_investment = 0.0  // 0 means use current portfolio value
    );

private:
    std::vector<StockData> historical_data_;
    const size_t MAX_PORTFOLIO_STOCKS = 3;
    const size_t SHORT_MA_PERIOD = 10;
    const size_t LONG_MA_PERIOD = 30;
    
    double predict_single_stock(const std::string& symbol, 
                              const std::string& date,
                              double& confidence_score) const;
    std::optional<StockData> find_stock_data(const std::string& symbol) const;
    std::vector<std::string> get_dates_before(const std::string& date, size_t days) const;
    std::vector<double> get_price_history(const StockData& stock, 
                                        const std::string& end_date,
                                        size_t days,
                                        bool use_closing_price = true) const;
    double calculate_sma(const std::vector<double>& prices, size_t period) const;
    double calculate_ema(const std::vector<double>& prices, size_t period) const;
    double get_current_price(const StockData& stock, const std::string& date) const;
};

}  // namespace stock_analyzer