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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE Debug)

# Find required packages
find_package(nlohmann_json CONFIG REQUIRED)  # For JSON processing
find_package(fmt CONFIG REQUIRED)            # For string formatting
find_package(spdlog CONFIG REQUIRED)         # For logging
find_package(CLI11 CONFIG REQUIRED)          # For command line parsing

# Add source files
set(SOURCES
    src/main.cpp
    src/data_processing/csv_parser.cpp
    src/data_processing/json_parser.cpp
    src/calculations/format.cpp
    src/calculations/lookup.cpp
    src/calculations/speculation.cpp
    src/calculations/rebalance.cpp
    src/calculations/strategies.cpp
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