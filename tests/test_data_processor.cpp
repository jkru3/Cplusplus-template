// tests/test_data_processor.cpp
#include <gtest/gtest.h>
#include "data_processor.hpp"
#include <filesystem>

class DataProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test data
        testData = {
            {"test", "value"},
            {"number", 42}
        };
    }
    
    void TearDown() override {
        // Clean up test files
        std::filesystem::remove("test.json");
    }
    
    DataProcessor processor;
    nlohmann::json testData;
};

TEST_F(DataProcessorTest, SaveAndLoadData) {
    EXPECT_TRUE(processor.saveData(testData, "test.json"));
    auto loaded = processor.loadData("test.json");
    EXPECT_EQ(loaded["test"], "value");
    EXPECT_EQ(loaded["number"], 42);
}

TEST_F(DataProcessorTest, HandleInvalidFile) {
    auto result = processor.loadData("nonexistent.json");
    EXPECT_TRUE(result.empty());
    EXPECT_FALSE(processor.getLastError().empty());
}