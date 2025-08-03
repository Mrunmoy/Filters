#!/usr/bin/env bash
set -e
BUILD_TYPE="${1:-Debug}"
cmake -S . -B build -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" -DBUILD_TESTING=ON && cmake --build build -j && ctest --test-dir build --output-on-failure
