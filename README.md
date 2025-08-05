# Filters

A collection of small, focused **filters** implemented in modern C++ with unit tests and optional data visualization.\
The original code was in Matlab (see Reference section below) by Dr. Shane Ross.\
I converted them to C++ for learning and experimentation.

- **Build system:** CMake (GoogleTest is fetched automatically)
- **Language standard:** C++17 (or newer)

---

## Layout

```
filters/
  CMakeLists.txt
  avg/
    README.md
    inc/
    src/
    test/
  lpf/
    README.md
    inc/
    src/
    test/
  kalman/
    README.md
    inc/
    src/
    test/
  utils/
    CsvData.hpp
    CsvData.cpp
    scripts/
      mat_to_csv.py
      plot.py
    data/
      SonarAlt.csv
      Voltage.csv
```

---

## Build & Test

From the repo root:

```bash
./build.sh
```

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```

---

## Filters

- [Average Filters (Running Mean & Moving Average)](avg/README.md)
- [Low-Pass Filter (First Order IIR)](lpf/README.md) – a recursive filter where the current output depends on the previous output and current input.
- [Simple Kalman Filter (1D Estimation)](kalman/README.md) – estimates the true value from noisy measurements using recursive Bayesian update.

---

## Python Plotting Requirements

Install Python packages for interactive or saved plots:

```bash
pip install -r utils/scripts/requirements.txt
```

To enable interactive plots (on Linux):

```bash
sudo apt-get install -y python3-tk
```

---

## Reference

- Original lectures & Matlab code by Dr. Shane Ross:\
  [Google Drive](https://drive.google.com/drive/folders/1oG4mPxUaJMPU0STWOipA1pOLXO_Wrcbk)

- Lecture video:\
  [YouTube](https://www.youtube.com/watch?v=HCd-leV8OkU)

---

## License

MIT License – see [LICENSE](LICENSE).
