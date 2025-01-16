#include "rebalance.hpp"
#include <stdexcept>

namespace stock_analyzer {

std::vector<RebalanceAction> 
PortfolioRebalancer::rebalance_portfolio(const Portfolio& portfolio,
                                        const std::vector<StockData>& stock_data,
                                        int period,
                                        int max_holdings,
                                        std::shared_ptr<Strategy> strategy) {
    if (!strategy) {
        throw std::runtime_error("error with strategy");
    }

    std::vector<RebalanceAction> actions;
    std::map<std::string, const StockData*> stock_map;
    
    // Build stock map for quick lookup
    for (const auto& stock : stock_data) {
        stock_map[stock.symbol] = &stock;
    }

    // Verify all holdings exist in dataset
    for (const auto& holding : portfolio.holdings) {
        if (stock_map.find(holding.ticker) == stock_map.end()) {
            throw std::runtime_error("a holding's ticker is not represented in dataset");
        }
    }

    // Get ranked list of stocks by projected performance
    auto ranked_stocks = rank_stocks(stock_data, portfolio.date, period, strategy);
    
    // Track current holdings and their quantities
    std::map<std::string, int> current_holdings;
    for (const auto& holding : portfolio.holdings) {
        current_holdings[holding.ticker] = holding.quantity;
    }

    // Calculate total portfolio value
    double total_value = portfolio.cash;
    for (const auto& holding : current_holdings) {
        const std::string& current_ticker = holding.first;
        int quantity = holding.second;
        double price = get_stock_price(*stock_map[current_ticker], portfolio.date);
        total_value += price * quantity;
    }

    // Sell underperforming stocks
    for (const auto& holding : current_holdings) {
        const std::string& current_ticker = holding.first;
        int quantity = holding.second;
        
        auto it = std::find_if(ranked_stocks.begin(), ranked_stocks.end(),
            [current_ticker](const auto& pair) { return pair.first == current_ticker; });
        
        if (it == ranked_stocks.end() || 
            std::distance(ranked_stocks.begin(), it) >= max_holdings) {
            // Stock should be sold
            RebalanceAction action;
            action.action_type = RebalanceAction::Type::SELL;
            action.ticker = current_ticker;
            action.quantity = quantity;
            
            const auto& stock = *stock_map[current_ticker];
            action.projected_change = strategy->project_change(stock, portfolio.date, period);
            
            double current_price = get_stock_price(stock, portfolio.date);
            action.projected_value = -(current_price * quantity);
            
            // Calculate actual performance if data available
            auto price_it = stock.daily_prices.find(portfolio.date);
            if (price_it != stock.daily_prices.end()) {
                // First check if we have enough elements ahead
                auto temp_it = price_it;
                bool has_enough_data = true;
                for (int i = 0; i < period && has_enough_data; ++i) {
                    if (std::next(temp_it) == stock.daily_prices.end()) {
                        has_enough_data = false;
                        break;
                    }
                    temp_it = std::next(temp_it);
                }
                
                if (has_enough_data) {
                    double end_price = temp_it->second.close;
                    action.actual_change = ((end_price - current_price) / current_price) * 100.0;
                    action.actual_value = -(end_price * quantity);
                    action.accuracy = 100.0 - std::abs(action.actual_change - action.projected_change);
                }
            }
            
            actions.push_back(action);
        }
    }

    // Buy promising stocks
    double available_cash = portfolio.cash;
    for (const auto& ranked_stock : ranked_stocks) {
        if (actions.size() >= max_holdings) break;
        
        const std::string& ticker = ranked_stock.first;
        double proj_change = ranked_stock.second;
        
        // Skip if we already hold this stock
        if (current_holdings.find(ticker) != current_holdings.end()) continue;
        
        const auto& stock = *stock_map[ticker];
        double current_price = get_stock_price(stock, portfolio.date);
        
        // Calculate how many shares we can buy
        int quantity = static_cast<int>(available_cash / (current_price * max_holdings));
        if (quantity > 0) {
            RebalanceAction action;
            action.action_type = RebalanceAction::Type::BUY;
            action.ticker = ticker;
            action.quantity = quantity;
            action.projected_change = proj_change;
            action.projected_value = current_price * quantity;
            
            // Calculate actual performance if data available
            auto price_it = stock.daily_prices.find(portfolio.date);
            if (price_it != stock.daily_prices.end()) {
                // First check if we have enough elements ahead
                auto temp_it = price_it;
                bool has_enough_data = true;
                for (int i = 0; i < period && has_enough_data; ++i) {
                    if (std::next(temp_it) == stock.daily_prices.end()) {
                        has_enough_data = false;
                        break;
                    }
                    temp_it = std::next(temp_it);
                }
                
                if (has_enough_data) {
                    double end_price = temp_it->second.close;
                    action.actual_change = ((end_price - current_price) / current_price) * 100.0;
                    action.actual_value = end_price * quantity;
                    action.accuracy = 100.0 - std::abs(action.actual_change - action.projected_change);
                }
            }
            
            actions.push_back(action);
            available_cash -= current_price * quantity;
        }
    }

    return actions;
}

// Rest of the implementation remains unchanged
double PortfolioRebalancer::get_stock_price(const StockData& stock, const std::string& date) {
    auto it = stock.daily_prices.find(date);
    if (it == stock.daily_prices.end()) {
        throw std::runtime_error("date out of bounds");
    }
    return it->second.close;
}

std::vector<std::pair<std::string, double>> 
PortfolioRebalancer::rank_stocks(const std::vector<StockData>& stock_data,
                                const std::string& date,
                                int period,
                                std::shared_ptr<Strategy> strategy) {
    std::vector<std::pair<std::string, double>> rankings;
    
    for (const auto& stock : stock_data) {
        try {
            double projected_change = strategy->project_change(stock, date, period);
            rankings.push_back({stock.symbol, projected_change});
        } catch (...) {
            // Skip stocks that can't be projected
            continue;
        }
    }
    
    // Sort by projected change in descending order
    std::sort(rankings.begin(), rankings.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    return rankings;
}

}  // namespace stock_analyzer