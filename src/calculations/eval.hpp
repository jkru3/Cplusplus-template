// src/calculations/eval.hpp
#pragma once
#include "common/types.hpp"
#include <string>

namespace stock_analyzer {

class StockEval {
public:
    static std::string format_lookup_result(const StockData& stock, const std::string& date);

    static std::string format_projection_header(bool include_actual = true);
    static std::string format_projection_summary(const ProjectionSummary& summary, bool include_actual = true);
    static std::string format_projection_result(const ProjectionResult& result, bool include_actual = true);
    
    static std::string format_rebalance_action(const RebalanceAction& action, bool include_actual = true);
};

}  // namespace stock_analyzer