// src/calculations/format.cpp
#include "format.hpp"
#include <sstream>
#include <iomanip>

namespace stock_analyzer {

std::string Format::format_lookup_result(const StockData& stock, const std::string& date) {
    auto it = stock.daily_prices.find(date);
    if (it == stock.daily_prices.end()) {
        return "";
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << std::left << std::setw(6) << stock.symbol << " | "
       << std::setw(18) << it->second.sector << " | "
       << std::setw(8) << it->second.open << " | "
       << std::setw(8) << it->second.high << " | "
       << std::setw(8) << it->second.low << " | "
       << std::setw(8) << it->second.close << " | "
       << std::setw(10) << std::setprecision(0) << it->second.volume;

    return ss.str();
}

std::string Format::format_projection_header(bool include_actual) {
    std::stringstream ss;
    ss << std::left << "Ticker" << " | "
       << " Proj % | ";
    
    if (include_actual) {
        ss << " Actual % | "
           << "Accuracy %";
    }
    ss << "\n";

    return ss.str();
}

std::string Format::format_projection_result(const SpeculationResult& result, bool include_actual) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << std::setw(6) << result.ticker << " | "
       << std::setw(6) << result.projected_change << "% | ";
    
    if (include_actual) {
        ss << std::setw(8) << result.actual_change << "% | "
           << std::setw(9) << result.accuracy << "%";
    }

    return ss.str();
}

std::string Format::format_projection_summary(const SpeculationSummary& summary, bool include_actual) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << std::setw(6) << summary.total_projected_change << "% | ";
    
    if (include_actual) {
        ss << std::setw(8) << summary.total_actual_change << "% | "
           << std::setw(9) << summary.total_accuracy << "%";
    }

    return ss.str();
};

std::string Format::format_rebalance_action(const RebalanceAction& action, bool include_actual) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    ss << (action.action_type == RebalanceAction::Type::SELL ? "SOLD: " : "BOUGHT: ")
       << action.ticker << " | "
       << action.projected_change << "% | ";
    
    if (include_actual) {
        ss << action.actual_change << "% | "
           << action.projected_value << " | "
           << action.actual_value << " | "
           << action.accuracy << "%";
    } else {
        ss << action.projected_value;
    }
    
    return ss.str();
}

} // namespace stock_analyzer