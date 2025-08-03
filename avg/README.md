# Average (Running Mean) Filter

This module implements a simple **running arithmetic mean** filter with persistent state—equivalent to the MATLAB persistent version [here](https://drive.google.com/drive/folders/1oHuf9X6Iy3tcf6dRBCJVKTFa-KRoBEkB).

## What it does (math)

Given the k-th sample x_k and the previous running average avg_{k-1}:

```
alpha_k = (k - 1) / k
avg_k   = alpha_k * avg_{k-1} + (1 - alpha_k) * x_k
```

Initial conditions used here:
- `k` starts at 1
- `avg_0 = 0` → on the first update, `avg_1 = x_1`

## Directory layout

```
avg/
  CMakeLists.txt
  inc/
    RunningAverageFilter.hpp
  src/
    RunningAverageFilter.cpp
  test/
    CMakeLists.txt
    RunningAverageFilterTests.cpp
    plot.py
```

## Build & test

From the **repository root** (where the top-level `CMakeLists.txt` lives):

```bash
./build.sh
```

To run only the avg tests:
```bash
ctest --test-dir build -R FilterAvgTests --output-on-failure
```

### CSV output location

The avg test writes a CSV called `avg_sim.csv` containing `t,xm,avg` columns.

- Typical path (when the test working directory is the test binary dir):
  ```
  build/avg/test/avg_sim.csv
  ```
- If unsure, locate it with:
  ```bash
  find build -name avg_sim.csv
  ```

> Tip: To fix the working directory for the test, set this in `avg/test/CMakeLists.txt`:
> ```cmake
> include(GoogleTest)
> gtest_discover_tests(FilterAvgTests
>     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
> )
> ```
> Then the CSV will always land in `build/avg/test/`.

## Plotting the results

The test’s `plot.py` script plots the CSV and **saves a PNG** next to the CSV by default.

### 1) Install Python deps

- For interactive plotting (optional but recommended on Linux):
  ```bash
  sudo apt-get install -y python3-tk
  ```
- Python packages:
  ```bash
  pip install matplotlib numpy
  ```

### 2) Run the plot script

```bash
# Save PNG next to the CSV as <csv_basename>_plot.png
python3 avg/test/plot.py build/avg/test/avg_sim.csv

# Or choose a specific output path
python3 avg/test/plot.py build/avg/test/avg_sim.csv -o build/avg/test/avg_plot.png
```

The script prints the absolute path to the generated PNG.

## API (C++)

Header: `avg/inc/RunningAverageFilter.hpp`

```cpp
namespace Filters { namespace Avg {

class RunningAverageFilter
{
public:
    RunningAverageFilter();

    // Feed one sample; returns updated average
    double update(double x);

    // Reset to initial state (avg=0, k=1)
    void reset();

    // Accessors
    double getAverage() const;
    std::uint64_t getCount() const;
};

}} // namespace Filters::Avg
```

- `update(x)` updates and returns the new running average.
- `reset()` restores the initial conditions.
- `getAverage()` returns the current running mean.
- `getCount()` returns how many samples have been incorporated.

## License

MIT. See the repository’s `LICENSE` file.