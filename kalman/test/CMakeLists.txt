add_executable(FilterKalmanTests
    SimpleKalmanFilterTests.cpp
)

target_link_libraries(FilterKalmanTests PRIVATE
    FilterKalman
    Utils
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FilterKalmanTests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
