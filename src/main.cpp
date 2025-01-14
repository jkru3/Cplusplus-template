// src/main.cpp
#include "data_processor.hpp"
#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Starting application");
    
    DataProcessor processor;
    
    // Create sample data
    nlohmann::json data = {
        {"name", "John Doe"},
        {"age", 30},
        {"skills", {"C++", "CMake", "vcpkg"}}
    };
    
    // Save data
    if (processor.saveData(data, "output.json")) {
        // Load and display data
        auto loaded_data = processor.loadData("output.json");
        spdlog::info("Loaded name: {}", loaded_data["name"]);
        spdlog::info("Loaded age: {}", loaded_data["age"]);
    } else {
        spdlog::error("Error: {}", processor.getLastError());
    }
    
    return 0;
}