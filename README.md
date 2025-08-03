# filters

A collection of small, focused **filters** implemented in modern C++ with per-filter unit tests.
Each filter lives in its own directory with `inc/`, `src/`, and `test/` folders.

- Build system: CMake, with GoogleTest fetched automatically for tests.

## Layout

```
filters/
  CMakeLists.txt
  avg/
    CMakeLists.txt
    inc/RunningAverageFilter.hpp
    src/RunningAverageFilter.cpp
    test/Tests.cpp
```

## Build & Test

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Adding a New Filter

- Create a new directory, e.g. `median/`, with the same `include/`, `src/`, `test/` subfolders.
- Add `add_subdirectory(median)` in the top-level `CMakeLists.txt`.
- Export headers under a consistent namespace, e.g. `Filters::Median`.
