#include <gtest/gtest.h>

#include <cstdlib>              // for std::getenv
#include <filesystem>
#include <vector>
#include <numeric>
#include <random>
#include <cmath>

#include "MovingAverageFilter.hpp"
#include "CsvData.hpp"          // <-- new helper

using Filters::Avg::MovingAverageFilter;

// Naïve MATLAB-style reference (O(n) shift + sum) to compare against
static double MovAvgFilterReference(double x, std::vector<double>& buf, bool& firstRun)
{
    if (firstRun)
    {
        std::fill(buf.begin(), buf.end(), x);
        firstRun = false;
    }
    else
    {
        const std::size_t n = buf.size();
        for (std::size_t m = 0; m < n - 1; ++m)
        {
            buf[m] = buf[m + 1];
        }
        buf[n - 1] = x;
    }
    const double sum = std::accumulate(buf.begin(), buf.end(), 0.0);
    return sum / static_cast<double>(buf.size());
}

TEST(MovingAverageFilter, FirstOutputEqualsFirstSample)
{
    MovingAverageFilter f(100);
    double y = f.update(12.34);
    EXPECT_DOUBLE_EQ(y, 12.34);
    EXPECT_DOUBLE_EQ(f.getAverage(), 12.34);
}

TEST(MovingAverageFilter, ConstantSignalRemainsConstant)
{
    MovingAverageFilter f(16);
    for (int i = 0; i < 50; ++i)
    {
        double y = f.update(5.0);
        EXPECT_NEAR(y, 5.0, 1e-12);
    }
}

TEST(MovingAverageFilter, MatchesMatlabNaiveForRandomSequence)
{
    const std::size_t N = 64;
    MovingAverageFilter f(N);

    // Random sequence with fixed seed for determinism
    std::mt19937 rng(123);
    std::normal_distribution<double> dist(0.0, 1.0);

    std::vector<double> buf(N); // reference buffer
    bool first = true;

    for (int i = 0; i < 1000; ++i)
    {
        const double x = dist(rng);
        const double y_ref = MovAvgFilterReference(x, buf, first);
        const double y = f.update(x);
        EXPECT_NEAR(y, y_ref, 1e-12);
    }
}

TEST(MovingAverageFilter, WindowChangeResetsToFirstRun)
{
    MovingAverageFilter f(8);
    (void)f.update(1.0);
    f.setWindowSize(4);
    // Next update fills the whole new window with x
    double y = f.update(7.0);
    EXPECT_DOUBLE_EQ(y, 7.0);
}

// MOVAVG_TEST_SOURCE_DIR is injected by CMake (see target_compile_definitions)
#ifndef MOVAVG_TEST_SOURCE_DIR
#  define MOVAVG_TEST_SOURCE_DIR "."
#endif

TEST(MovingAverageFilter, SimulationFromCsv)
{
    // 1) Try env var (user override)
    const char* csvEnv = std::getenv("MOVAVG_SIM_CSV");

    // 2) Fallback to default file in the test *source* folder
    std::string csvPath = csvEnv
        ? std::string(csvEnv)
        : (std::string(MOVAVG_TEST_SOURCE_DIR) + "/SonarAlt.csv");

    // Optional knobs (still allow env overrides)
    const std::size_t win = [&](){
        if (const char* w = std::getenv("MOVAVG_WIN")) return static_cast<std::size_t>(std::stoul(w));
        return static_cast<std::size_t>(10);
    }();
    const std::string tcol = [](){ if (const char* v = std::getenv("MOVAVG_TCOL")) return std::string(v); return std::string("t"); }();
    const std::string ycol = [](){ if (const char* v = std::getenv("MOVAVG_YCOL")) return std::string(v); return std::string("z"); }();

    // If the default file doesn't exist, skip with a helpful message
    if (!std::filesystem::exists(csvPath))
    {
        GTEST_SKIP() << "Input CSV not found at '" << csvPath
                     << "'. Set MOVAVG_SIM_CSV to point to your CSV, or place 'sonar.csv' in "
                     << MOVAVG_TEST_SOURCE_DIR << ".";
    }

    // Load, filter, write, and check
    CsvSeries s = CsvIO::Load(csvPath, tcol, ycol);
    ASSERT_FALSE(s.y.empty()) << "CSV had no rows: " << csvPath;

    MovingAverageFilter f(win);
    std::vector<double> yavg; yavg.reserve(s.y.size());
    for (double v : s.y) yavg.push_back(f.update(v));

    std::filesystem::path in(csvPath);
    std::filesystem::path out = in.parent_path() / (in.stem().string() + "_movavg_out.csv");
    CsvIO::Write3(out.string(), s.t, s.y, yavg, "t", "y", "avg");

    const double stdIn  = CsvIO::StdDev(s.y);
    const double stdOut = CsvIO::StdDev(yavg);
    EXPECT_LT(stdOut, stdIn) << "Expected std(avg) < std(raw)";
}
