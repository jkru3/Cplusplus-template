// src/data_processing/json_parser.hpp
#pragma once
#include "common/types.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace stock_analyzer {

class JSONParser {
public:
    static Portfolio parse_portfolio_json(const std::string& filename);
};

}  // namespace stock_analyzer