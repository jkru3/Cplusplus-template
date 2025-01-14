// src/data_processor.cpp
#include "data_processor.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

DataProcessor::DataProcessor() {
    spdlog::set_level(spdlog::level::debug);
}

bool DataProcessor::saveData(const nlohmann::json& data, const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file) {
            lastError = "Could not open file for writing: " + filename;
            spdlog::error(lastError);
            return false;
        }
        
        file << std::setw(4) << data << std::endl;
        spdlog::info("Successfully saved data to {}", filename);
        return true;
    } catch (const std::exception& e) {
        lastError = std::string("Error saving data: ") + e.what();
        spdlog::error(lastError);
        return false;
    }
}

nlohmann::json DataProcessor::loadData(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file) {
            lastError = "Could not open file for reading: " + filename;
            spdlog::error(lastError);
            return nlohmann::json();
        }
        
        nlohmann::json data;
        file >> data;
        spdlog::info("Successfully loaded data from {}", filename);
        return data;
    } catch (const std::exception& e) {
        lastError = std::string("Error loading data: ") + e.what();
        spdlog::error(lastError);
        return nlohmann::json();
    }
}

std::string DataProcessor::getLastError() const {
    return lastError;
}