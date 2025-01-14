// include/data_processor.hpp
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class DataProcessor {
public:
    DataProcessor();
    
    // Process and save JSON data
    bool saveData(const nlohmann::json& data, const std::string& filename);
    
    // Load and parse JSON data
    nlohmann::json loadData(const std::string& filename);
    
    // Get the last error message
    std::string getLastError() const;

private:
    std::string lastError;
};