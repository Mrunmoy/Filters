add_executable(FilterLpfTests
    LowPassFilterTests.cpp
)

target_link_libraries(FilterLpfTests PRIVATE
    FilterLpf
    Utils
    GTest::gtest_main
)

target_include_directories(FilterLpfTests PRIVATE
    ${CMAKE_SOURCE_DIR}/utils  # So CsvData.hpp is found
)

include(GoogleTest)
gtest_discover_tests(FilterLpfTests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
