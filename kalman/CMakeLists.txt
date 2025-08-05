add_library(FilterKalman
    src/SimpleKalmanFilter.cpp
)

target_include_directories(FilterKalman PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Tests
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
