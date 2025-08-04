add_library(Utils
    CsvData.cpp
)

target_include_directories(Utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Expose the path to the data directory as a preprocessor macro
target_compile_definitions(Utils PUBLIC
    DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

