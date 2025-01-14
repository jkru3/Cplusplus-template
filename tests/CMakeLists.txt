# tests/CMakeLists.txt
add_executable(${PROJECT_NAME}_tests
    tests/test_data_processor.cpp
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)