cmake_minimum_required(VERSION 3.20)

add_library(FilterAvg
    src/RunningAverageFilter.cpp
    src/MovingAverageFilter.cpp
)

target_include_directories(FilterAvg PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
