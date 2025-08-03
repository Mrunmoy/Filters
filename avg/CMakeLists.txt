cmake_minimum_required(VERSION 3.20)

add_library(FilterAvg
    src/RunningAverageFilter.cpp
    src/MovingAverageFilter.cpp
)
target_include_directories(FilterAvg PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Tests for avg
if (BUILD_TESTING)
  add_executable(FilterAvgTests
      test/RunningAverageFilterTests.cpp
      test/MovingAverageFilterTests.cpp
      test/CsvData.cpp # <- shared test utility
  )

  target_include_directories(FilterAvgTests PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/test
  )

  # Inject the absolute path of the *test source* directory
  target_compile_definitions(FilterAvgTests PRIVATE
      MOVAVG_TEST_SOURCE_DIR="${CMAKE_CURRENT_LIST_DIR}/test"
  )

  target_link_libraries(FilterAvgTests PRIVATE
      FilterAvg
      GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(FilterAvgTests
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}   # e.g., build/avg/
      DISCOVERY_TIMEOUT 30
  )
endif()
