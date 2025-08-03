# Filters

A collection of small, focused **filters** implemented in modern C++ with unit tests and optional data visualization. The original code was in Matlab (refer to Reference section below) from Dr. Shane Ross. I just converted them to C++ version for my own understanding and playing with them.

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
  median/
    README.md
    inc/
    src/
    test/
  ...
```

---

## Build & Test

```bash
./build.sh
```

---

## Filters

- [Average Filters (Running Mean & Moving Average)](avg/README.md)

*(More filters will be listed here as they are added.)*

---

## Reference
https://drive.google.com/drive/folders/1oG4mPxUaJMPU0STWOipA1pOLXO_Wrcbk

https://www.youtube.com/watch?v=HCd-leV8OkU&list=PLUeHTafWecAXDFDYEwunLL2V2kwqKzkvJ&index=23


## License

MIT License – see [LICENSE](LICENSE).