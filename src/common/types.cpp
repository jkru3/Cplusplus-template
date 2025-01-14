// src/common/types.cpp
#include "common/types.hpp"

namespace stock_analyzer {

void to_json(nlohmann::json& j, const StockData& s) {
    j = nlohmann::json{
        {"symbol", s.symbol},
        {"daily_prices", s.daily_prices},
        {"debt_equity_ratio", s.debt_equity_ratio},
        {"market_cap", s.market_cap}
    };
}

void from_json(const nlohmann::json& j, StockData& s) {
    j.at("symbol").get_to(s.symbol);
    j.at("daily_prices").get_to(s.daily_prices);
    j.at("debt_equity_ratio").get_to(s.debt_equity_ratio);
    j.at("market_cap").get_to(s.market_cap);
}

void to_json(nlohmann::json& j, const Portfolio& p) {
    j = nlohmann::json{
        {"id", p.id},
        {"holdings", p.holdings}
    };
}

void from_json(const nlohmann::json& j, Portfolio& p) {
    j.at("id").get_to(p.id);
    j.at("holdings").get_to(p.holdings);
}

}  // namespace stock_analyzer