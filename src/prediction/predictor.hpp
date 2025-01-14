// src/prediction/predictor.hpp
#pragma once
#include "common/types.hpp"

namespace stock_analyzer {

class Predictor {
public:
    Predictor(const std::vector<StockData>& historical_data);
    PredictionResult predict_portfolio(const Portfolio& portfolio, 
                                     const std::string& start_date);

private:
    std::vector<StockData> historical_data_;
    double predict_single_stock(const std::string& symbol, 
                              const std::string& date);
};

}  // namespace stock_analyzer