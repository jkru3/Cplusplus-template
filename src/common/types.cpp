// src/common/types.cpp
#include "common/types.hpp"

// namespace stock_analyzer {

// void to_json(nlohmann::json& j, const StockPrice& p) {
//     j = nlohmann::json{
//         {"date", p.date},
//         {"sector", p.sector},
//         {"open", p.open},
//         {"high", p.high},
//         {"low", p.low},
//         {"close", p.close},
//         {"volume", p.volume}
//     };
// }

// void from_json(const nlohmann::json& j, StockPrice& p) {
//     j.at("date").get_to(p.date);
//     j.at("sector").get_to(p.sector);
//     j.at("open").get_to(p.open);
//     j.at("high").get_to(p.high);
//     j.at("low").get_to(p.low);
//     j.at("close").get_to(p.close);
//     j.at("volume").get_to(p.volume);
// }

// void to_json(nlohmann::json& j, const StockData& s) {
//     j = nlohmann::json{
//         {"symbol", s.symbol},
//         {"sector", s.sector},
//         {"daily_prices", s.daily_prices}
//     };
// }

// void from_json(const nlohmann::json& j, StockData& s) {
//     j.at("symbol").get_to(s.symbol);
//     j.at("sector").get_to(s.sector);
//     j.at("daily_prices").get_to(s.daily_prices);
// }

// void to_json(nlohmann::json& j, const PortfolioHolding& h) {
//     j = nlohmann::json{
//         {"ticker", h.ticker},
//         {"quantity", h.quantity}
//     };
// }

// void from_json(const nlohmann::json& j, PortfolioHolding& h) {
//     j.at("ticker").get_to(h.ticker);
//     j.at("quantity").get_to(h.quantity);
// }

// void to_json(nlohmann::json& j, const Portfolio& p) {
//     j = nlohmann::json{
//         {"id", p.id},
//         {"date", p.date},
//         {"cash", p.cash},
//         {"holdings", p.holdings}
//     };
// }

// void from_json(const nlohmann::json& j, Portfolio& p) {
//     j.at("id").get_to(p.id);
//     j.at("date").get_to(p.date);
//     j.at("cash").get_to(p.cash);
//     j.at("holdings").get_to(p.holdings);
// }

// }  // namespace stock_analyzer