// tests/test_predictor.cpp
#include <gtest/gtest.h>
#include <filesystem>
#include "data_processing/stock_processor.hpp"
#include "data_processing/portfolio_generator.hpp"
#include "prediction/predictor.hpp"
#include <fstream>

namespace fs = std::filesystem;

class PredictorTest : public ::testing::Test {
protected:
    fs::path test_data_dir;
    fs::path output_dir;

    void SetUp() override {
        // Create test directories
        test_data_dir = fs::temp_directory_path() / "test_data";
        output_dir = fs::temp_directory_path() / "test_output";
        fs::create_directories(test_data_dir);
        fs::create_directories(output_dir);

        // Create mock data file
        std::ofstream mock_data(test_data_dir / "processed_stocks.json");
        mock_data << R"({
            "stocks": [
                {
                    "symbol": "AAPL",
                    "data": [
                        {
                            "date": "2024-01-01",
                            "open": 150.0,
                            "close": 152.5,
                            "high": 153.0,
                            "low": 149.5,
                            "volume": 1000000
                        }
                    ]
                }
            ]
        })";
        mock_data.close();
    }

    void TearDown() override {
        // Clean up test directories
        fs::remove_all(test_data_dir);
        fs::remove_all(output_dir);
    }
};

TEST_F(PredictorTest, TestDataLoading) {
    stock_analyzer::StockProcessor processor;
    auto stock_data = processor.load_processed_data(test_data_dir / "processed_stocks.json");
    
    ASSERT_FALSE(stock_data.empty());
    EXPECT_EQ(stock_data[0].symbol, "AAPL");
    EXPECT_EQ(stock_data[0].data.size(), 1);
}

TEST_F(PredictorTest, TestPrediction) {
    stock_analyzer::StockProcessor processor;
    stock_analyzer::Predictor predictor;
    
    auto stock_data = processor.load_processed_data(test_data_dir / "processed_stocks.json");
    auto prediction = predictor.predict(stock_data[0]);
    
    EXPECT_FALSE(std::isnan(prediction.predicted_price));
    EXPECT_GT(prediction.confidence, 0.0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}