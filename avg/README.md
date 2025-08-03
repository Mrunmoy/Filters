# Average Filters (Running Mean & Moving Average)

This module implements two common averaging filters with stateful behavior, equivalent to their MATLAB implementations:

1. **Running Arithmetic Mean Filter** (persistent state, incremental mean)  
2. **Fixed-Window Moving Average Filter** (sliding window of N samples)

---

## 1️⃣ Running Arithmetic Mean Filter

Mathematically:

$$
\alpha_k = \frac{k-1}{k}, \qquad
A_k = \alpha_k \cdot A_{k-1} + (1-\alpha_k) \cdot x_k
$$

- Initial state:  
  - `k = 1`  
  - `avg_0 = 0` → on first update, `avg_1 = x_1`  

---

## 2️⃣ Moving Average Filter (Window N)

For a signal sequence \( x_k \), with window size \( N \):


$$
A_k = \frac{1}{N}\sum_{i=k-N+1}^{k} x_i
$$

- The first call fills the entire buffer with the first sample (MATLAB behavior).  
- Internally implemented with a **ring buffer** for efficiency (\(O(1)\) update).

---

## Directory Layout

```
avg/
  CMakeLists.txt
  inc/
    RunningAverageFilter.hpp
    MovingAverageFilter.hpp
  src/
    RunningAverageFilter.cpp
    MovingAverageFilter.cpp
  test/
    CMakeLists.txt
    RunningAverageFilterTests.cpp
    MovingAverageFilterTests.cpp
    CsvData.cpp / CsvData.hpp
    plot.py
    sonar.csv                # Optional: input data for MovingAvg sim
```

---

## Build & Run Tests

From the **repo root**:

```bash
./build.sh
```

Run only tests for this module:

```bash
ctest --test-dir build -R FilterAvgTests --output-on-failure
```

---

### Test CSV Outputs

- **Running Mean Filter:**  
  Writes `avg_sim.csv` with columns: `t,xm,avg`  
  Typical location:
  ```
  build/avg/test/avg_sim.csv
  ```

- **Moving Average Filter (Simulation):**  
  - Input CSV defaults to `avg/test/sonar.csv` unless `MOVAVG_SIM_CSV` is set.
  - Output CSV: `<input_stem>_movavg_out.csv` with columns: `t,y,avg`  
    e.g.:
  ```
  avg/test/sonar_movavg_out.csv
  ```
  or
  ```
  build/avg/sonar_movavg_out.csv
  ```

---

## Plotting Results

The shared plot script will visualize both filters.

### 1) Install Python deps

```bash
sudo apt-get install -y python3-tk   # for interactive plot windows
pip install matplotlib numpy
```

### 2) Plot

Example for running mean:
```bash
python3 avg/test/plot.py build/avg/test/avg_sim.csv
```

Example for moving average:
```bash
python3 avg/test/plot.py avg/test/sonar_movavg_out.csv -x t -y y avg --title "Moving Average Filter"
```

Plots are saved as PNG next to the CSV and path is printed:
```
Saved PNG: /absolute/path/to/sonar_movavg_out_plot.png
```

---

## API Overview

### RunningAverageFilter
Header: `avg/inc/RunningAverageFilter.hpp`
```cpp
double update(double x);
void reset();
double getAverage() const;
std::uint64_t getCount() const;
```

### MovingAverageFilter
Header: `avg/inc/MovingAverageFilter.hpp`
```cpp
explicit MovingAverageFilter(std::size_t windowSize = 100);
double update(double x);
void reset();
void setWindowSize(std::size_t n);
double getAverage() const;
std::size_t getWindowSize() const;
```

---

## License

MIT. See [LICENSE](../LICENSE).
