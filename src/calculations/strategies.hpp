// src/calculations/strategies.hpp
#pragma once
#include "common/types.hpp"
#include <string>

namespace stock_analyzer {

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual double project_change(const StockData& data, const std::string& start_date, int period) = 0;
};

class DefaultStrategy : public Strategy {
public:
    double project_change(const StockData& data, const std::string& start_date, int period) override;
private:
    std::string get_date_before(const std::map<std::string, StockPrice>& prices, const std::string& date, int days);
};

class RandomStrategy : public Strategy {
public:
    double project_change(const StockData& data, const std::string& start_date, int period) override;
};

}  // namespace stock_analyzer