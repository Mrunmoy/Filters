cmake_minimum_required(VERSION 3.20)

add_library(FilterLpf
    src/LowPassFilter.cpp
)

target_include_directories(FilterLpf PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
