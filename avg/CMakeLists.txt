# avg/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

add_library(FilterAvg
    src/RunningAverageFilter.cpp
)
target_include_directories(FilterAvg PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Tests for avg
if(BUILD_TESTING)
  enable_testing()
  add_executable(FilterAvgTests
      test/RunningAverageFilterTests.cpp
  )
  target_link_libraries(FilterAvgTests PRIVATE
      FilterAvg
      GTest::gtest_main
  )
  include(GoogleTest)
  gtest_discover_tests(FilterAvgTests)
endif()
