// src/prediction/predictor.cpp
#include "prediction/predictor.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <iomanip>

namespace stock_analyzer {

Predictor::Predictor(const std::vector<StockData>& historical_data)
    : historical_data_(historical_data) {}

std::optional<StockData> Predictor::find_stock_data(const std::string& symbol) const {
    auto it = std::find_if(historical_data_.begin(), historical_data_.end(),
        [&symbol](const StockData& data) { return data.symbol == symbol; });
    
    if (it != historical_data_.end()) {
        return *it;
    }
    return std::nullopt;
}

double Predictor::calculate_sma(const std::vector<double>& prices, size_t period) const {
    if (prices.size() < period) return 0.0;
    
    double sum = std::accumulate(prices.begin(), prices.begin() + period, 0.0);
    return sum / period;
}

double Predictor::calculate_ema(const std::vector<double>& prices, size_t period) const {
    if (prices.size() < period) return 0.0;
    
    double multiplier = 2.0 / (period + 1.0);
    double ema = prices[0];
    
    for (size_t i = 1; i < period; ++i) {
        ema = (prices[i] - ema) * multiplier + ema;
    }
    
    return ema;
}

double Predictor::get_current_price(const StockData& stock, const std::string& date) const {
    auto it = stock.daily_prices.find(date);
    if (it != stock.daily_prices.end()) {
        return it->second.close;
    }
    return 0.0;
}

std::vector<std::string> Predictor::get_dates_before(const std::string& date, size_t days) const {
    std::vector<std::string> result;
    
    // Convert string date to tm structure
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    
    // Convert to time_t
    std::time_t time = std::mktime(&tm);
    
    // Get previous dates
    for (size_t i = 0; i < days; ++i) {
        time -= 86400; // Subtract one day in seconds
        tm = *std::localtime(&time);
        
        std::ostringstream date_ss;
        date_ss << std::put_time(&tm, "%Y-%m-%d");
        result.push_back(date_ss.str());
    }
    
    return result;
}

std::vector<double> Predictor::get_price_history(const StockData& stock, 
                                               const std::string& end_date,
                                               size_t days,
                                               bool use_closing_price) const {
    std::vector<double> prices;
    auto dates = get_dates_before(end_date, days);
    
    for (const auto& date : dates) {
        auto price_it = stock.daily_prices.find(date);
        if (price_it != stock.daily_prices.end()) {
            prices.push_back(use_closing_price ? price_it->second.close : price_it->second.open);
        }
    }
    
    return prices;
}

double Predictor::predict_single_stock(const std::string& symbol, 
                                       const std::string& date,
                                       double& confidence_score) const {
    auto stock_data = find_stock_data(symbol);
    if (!stock_data.has_value()) {
        spdlog::warn("Stock data not found for symbol: {}", symbol);
        return 0.0;
    }
    
    auto stock = stock_data.value();
    auto current_price = get_current_price(stock, date);
    if (current_price == 0.0) {
        spdlog::warn("Current price not found for symbol: {}", symbol);
        return 0.0;
    }
    
    // Get price history for short and long moving averages
    auto short_prices = get_price_history(stock, date, SHORT_MA_PERIOD);
    auto long_prices = get_price_history(stock, date, LONG_MA_PERIOD);
    
    // Calculate moving averages
    double short_sma = calculate_sma(short_prices, SHORT_MA_PERIOD);
    double long_sma = calculate_sma(long_prices, LONG_MA_PERIOD);
    double short_ema = calculate_ema(short_prices, SHORT_MA_PERIOD);
    double long_ema = calculate_ema(long_prices, LONG_MA_PERIOD);
    
    // Predict price based on moving average crossover
    double predicted_price = 0.0;
    if (short_ema > long_ema) {
        predicted_price = short_sma;
        confidence_score = 0.8;
    } else {
        predicted_price = long_sma;
        confidence_score = 0.7;
    }
    
    return predicted_price;
}

std::vector<StockPrediction> Predictor::analyze_all_stocks(const std::string& date) {
    std::vector<StockPrediction> predictions;
    
    for (const auto& stock : historical_data_) {
        double confidence_score;
        double current_price = get_current_price(stock, date);
        if (current_price == 0.0) continue;
        
        double predicted_price = predict_single_stock(stock.symbol, date, confidence_score);
        if (predicted_price == 0.0) continue;
        
        double predicted_return = ((predicted_price - current_price) / current_price) * 100.0;
        
        predictions.push_back({
            stock.symbol,
            stock.sector,
            current_price,
            predicted_price,
            predicted_return,
            confidence_score
        });
    }
    
    // Sort by predicted return (highest to lowest)
    std::sort(predictions.begin(), predictions.end(),
              [](const auto& a, const auto& b) {
                  return a.predicted_return > b.predicted_return;
              });
    
    return predictions;
}

std::vector<PortfolioRecommendation> Predictor::recommend_portfolio_adjustments(
    const Portfolio& portfolio,
    double max_investment) {
    
    // Get predictions for all stocks
    auto all_predictions = analyze_all_stocks(portfolio.date);
    if (all_predictions.empty()) {
        spdlog::error("No valid predictions available");
        return {};
    }
    
    // Calculate current portfolio value
    double current_portfolio_value = portfolio.cash;
    std::map<std::string, int> current_holdings;
    
    for (const auto& holding : portfolio.holdings) {
        current_holdings[holding.ticker] = holding.quantity;
        auto it = std::find_if(all_predictions.begin(), all_predictions.end(),
                              [&](const auto& pred) { return pred.ticker == holding.ticker; });
        if (it != all_predictions.end()) {
            current_portfolio_value += it->current_price * holding.quantity;
        }
    }
    
    // If max_investment not specified, use current portfolio value
    if (max_investment <= 0.0) {
        max_investment = current_portfolio_value;
    }
    
    // Filter predictions to top 3 stocks by predicted return and confidence
    std::vector<StockPrediction> top_predictions;
    for (const auto& pred : all_predictions) {
        if (pred.confidence_score >= 0.7) {  // Only consider predictions with high confidence
            top_predictions.push_back(pred);
            if (top_predictions.size() >= MAX_PORTFOLIO_STOCKS) break;
        }
    }
    
    // Generate recommendations
    std::vector<PortfolioRecommendation> recommendations;
    
    // Calculate target allocation for each stock
    double allocation_per_stock = max_investment / MAX_PORTFOLIO_STOCKS;
    
    // First, handle existing holdings that aren't in top predictions
    for (const auto& [ticker, quantity] : current_holdings) {
        auto it = std::find_if(top_predictions.begin(), top_predictions.end(),
                              [ticker](const auto& pred) { return pred.ticker == ticker; });
        if (it == top_predictions.end()) {
            recommendations.push_back({
                ticker,
                quantity,
                0,
                "SELL",
                0.0
            });
        }
    }
    
    // Then, generate recommendations for top predictions
    for (const auto& pred : top_predictions) {
        int current_qty = current_holdings[pred.ticker];
        int target_qty = static_cast<int>(allocation_per_stock / pred.current_price);
        
        std::string action;
        if (target_qty > current_qty) {
            action = "BUY";
        } else if (target_qty < current_qty) {
            action = "SELL";
        } else {
            action = "HOLD";
        }
        
        recommendations.push_back({
            pred.ticker,
            current_qty,
            target_qty,
            action,
            pred.predicted_return
        });
    }
    
    return recommendations;
}

}  // namespace stock_analyzer