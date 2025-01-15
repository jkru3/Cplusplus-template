// src/calculations/projection.cpp
#include "projection.hpp"
#include <stdexcept>
#include <cmath>

namespace stock_analyzer {

std::pair<std::vector<ProjectionResult>, ProjectionSummary>
StockProjection::project_stocks(const std::vector<StockData>& data,
                              const std::string& date,
                              int period,
                              std::shared_ptr<ProjectionStrategy> strategy) {
    if (!strategy) {
        throw std::runtime_error("error with strategy");
    }

    std::vector<ProjectionResult> results;
    ProjectionSummary summary = {0, 0, 0};
    bool found_any = false;

    for (const auto& stock : data) {
        auto start_it = stock.daily_prices.find(date);
        if (start_it == stock.daily_prices.end()) {
            continue;
        }
        found_any = true;

        ProjectionResult result;
        result.ticker = stock.symbol;

        try {
            result.projected_change = strategy->project_change(stock, date, period);
        } catch (...) {
            throw std::runtime_error("error with strategy");
        }

        double start_price = start_it->second.close;

        // Check if we can calculate actual changes
        auto future_date_it = std::next(start_it, period);
        if (future_date_it != stock.daily_prices.end()) {
            double end_price = future_date_it->second.close;
            result.actual_change = ((end_price - start_price) / start_price) * 100.0;
            result.accuracy = 100.0 - std::abs(result.actual_change - result.projected_change);
            
            // Update summary
            summary.total_projected_change += result.projected_change;
            summary.total_actual_change += result.actual_change;
            summary.total_accuracy += result.accuracy;
        }

        results.push_back(result);
    }

    if (!found_any) {
        throw std::runtime_error("date out of bounds");
    }

    // Calculate averages for summary
    int count = results.size();
    if (count > 0) {
        summary.total_projected_change /= count;
        summary.total_actual_change /= count;
        summary.total_accuracy /= count;
    }

    return {results, summary};
};

}  // namespace stock_analyzer