cmake_minimum_required(VERSION 3.20)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(cpp_prototype_template VERSION 1.0.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set vcpkg triplet
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx")

# Options
option(BUILD_TESTS "Build test suite" ON)

# Find dependencies through vcpkg
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Add source files
set(SOURCES
    src/main.cpp
    src/data_processor.cpp
)

# Add header files
set(HEADERS
    include/data_processor.hpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

# Tests
if(BUILD_TESTS)
    add_executable(${PROJECT_NAME}_tests
        test_data_processor.cpp
    )

    target_link_libraries(${PROJECT_NAME}_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        nlohmann_json::nlohmann_json
        spdlog::spdlog
    )

    target_include_directories(${PROJECT_NAME}_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
endif()

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)