add_executable(FilterAvgTests
    RunningAverageFilterTests.cpp
)

target_link_libraries(FilterAvgTests PRIVATE
    FilterAvg           # <- inherits PUBLIC include dir from FilterAvg
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FilterAvgTests)
