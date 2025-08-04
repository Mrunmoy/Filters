add_executable(RunningAverageFilterTests
    RunningAverageFilterTests.cpp
)

target_link_libraries(RunningAverageFilterTests PRIVATE
    FilterAvg           # <- inherits PUBLIC include dir from FilterAvg
    GTest::gtest_main
)

add_executable(MovingAverageFilterTests
    MovingAverageFilterTests.cpp
)
target_link_libraries(MovingAverageFilterTests PRIVATE
    FilterAvg
    Utils
    GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(RunningAverageFilterTests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
gtest_discover_tests(MovingAverageFilterTests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
