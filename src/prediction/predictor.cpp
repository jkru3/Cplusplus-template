// src/prediction/predictor.cpp
#include "prediction/predictor.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>

namespace stock_analyzer {

Predictor::Predictor(const std::vector<StockData>& historical_data)
    : historical_data_(historical_data) {}

PredictionResult Predictor::predict_portfolio(const Portfolio& portfolio, 
                                            const std::string& start_date) {
    PredictionResult result;
    result.portfolio_id = portfolio.id;
    
    double portfolio_predicted_value = 0.0;
    double portfolio_actual_value = 0.0;
    
    // For each stock in the portfolio
    for (const auto& [symbol, weight] : portfolio.holdings) {
        // Find the stock data
        auto it = std::find_if(historical_data_.begin(), historical_data_.end(),
            [&symbol](const StockData& data) { return data.symbol == symbol; });
            
        if (it != historical_data_.end()) {
            // Predict price for this stock
            double predicted_price = predict_single_stock(symbol, start_date);
            result.predicted_prices[symbol] = predicted_price;
            
            // Calculate contribution to portfolio value
            portfolio_predicted_value += predicted_price * weight;
            
            // Get actual price from historical data (14 days later)
            // This would need to look up the actual price from historical_data_
            // Implementation details...
        }
    }
    
    // Calculate returns and accuracy
    result.predicted_return = (portfolio_predicted_value - 1.0) * 100.0;  // as percentage
    result.actual_return = (portfolio_actual_value - 1.0) * 100.0;  // as percentage
    result.accuracy_score = 100.0 - std::abs(result.predicted_return - result.actual_return);
    
    return result;
}

double Predictor::predict_single_stock(const std::string& symbol, 
                                     const std::string& date) {
    // Implement your prediction algorithm here
    // This could use various techniques:
    // - Moving averages
    // - Linear regression
    // - Machine learning models
    // - Technical indicators
    
    // For now, returning a simple moving average
    double sum = 0.0;
    int count = 0;
    
    // Find the stock data
    auto it = std::find_if(historical_data_.begin(), historical_data_.end(),
        [&symbol](const StockData& data) { return data.symbol == symbol; });
        
    if (it != historical_data_.end()) {
        // Calculate 10-day moving average
        // This is a simplified example - you'd want to implement your actual
        // prediction algorithm here
        for (const auto& [day, price] : it->daily_prices) {
            if (count < 10) {
                sum += price;
                count++;
            } else {
                break;
            }
        }
    }
    
    return (count > 0) ? sum / count : 0.0;
}

}  // namespace stock_analyzer