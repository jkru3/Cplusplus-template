cmake_minimum_required(VERSION 3.15)

# Set vcpkg toolchain file
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
else()
    message(FATAL_ERROR "Please set VCPKG_ROOT environment variable to your vcpkg installation directory")
endif()

# Set the vcpkg triplet for Apple Silicon
if(APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(VCPKG_TARGET_TRIPLET "arm64-osx")
endif()

project(stock_analyzer VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# Find required packages
find_package(nlohmann_json CONFIG REQUIRED)  # For JSON processing
find_package(fmt CONFIG REQUIRED)            # For string formatting
find_package(spdlog CONFIG REQUIRED)         # For logging
find_package(CLI11 CONFIG REQUIRED)          # For command line parsing

# Add source files
set(SOURCES
    src/main.cpp
    src/common/types.cpp
    src/data_processing/csv_parser.cpp
    src/data_processing/stock_processor.cpp
    src/data_processing/portfolio_generator.cpp
    src/prediction/predictor.cpp
)

# Create executable
add_executable(stock_analyzer ${SOURCES})

# Include directories
target_include_directories(stock_analyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(stock_analyzer PRIVATE
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    CLI11::CLI11
)

# # Testing configuration
# enable_testing()
# include(FetchContent)

# # Fetch Google Test
# FetchContent_Declare(
#     googletest
#     GIT_REPOSITORY https://github.com/google/googletest.git
#     GIT_TAG release-1.12.1
# )
# FetchContent_MakeAvailable(googletest)

# # Add test executable
# add_executable(test_predictor
#     tests/test_predictor.cpp
# )

# target_include_directories(test_predictor PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/tests
# )

# target_link_libraries(test_predictor PRIVATE
#     GTest::gtest
#     GTest::gtest_main
#     CLI11::CLI11
#     spdlog::spdlog
# )

# # Register tests
# include(GoogleTest)
# gtest_discover_tests(test_predictor)