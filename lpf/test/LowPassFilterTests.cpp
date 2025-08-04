#include <gtest/gtest.h>
#include "LowPassFilter.hpp"
#include <vector>
#include <fstream>
#include <filesystem>

#include "CsvData.hpp"          // <-- new helper

using Filters::LPF::LowPassFilter;

TEST(LowPassFilter, SanityAndCSVWrite)
{
    constexpr double alpha = 0.2;
    LowPassFilter filter(alpha);

    const double dt = 0.2;
    const double duration = 10.0;
    const int N = static_cast<int>(duration / dt) + 1;

    std::vector<double> t(N);
    std::vector<double> xm(N);
    std::vector<double> xlpf(N);

    for (int i = 0; i < N; ++i)
    {
        t[i] = i * dt;
        xm[i] = 14.4 + 4.0 * ((double)rand() / RAND_MAX - 0.5) * 2.0;  // noisy
        xlpf[i] = filter.update(xm[i]);
    }

    // Write to CSV
    std::filesystem::create_directories("build/lpf/test");

    std::ofstream file("build/lpf/test/lpf_sim.csv");
    file << "t,xm,xlpf\n";
    for (int i = 0; i < N; ++i)
    {
        file << t[i] << "," << xm[i] << "," << xlpf[i] << "\n";
    }

    file.close();
    SUCCEED(); // If we made it here, we're good
}

#ifndef LPF_SIM_CSV
#define LPF_SIM_CSV "lpf_sim.csv"
#endif

TEST(LowPassFilter, SimulateWithSonarAlt)
{
    namespace fs = std::filesystem;
    using Filters::LPF::LowPassFilter;

    const std::string csvPath = std::string(DATA_DIR) + "/SonarAlt.csv";

    // Optional knobs
    const double alpha = [](){
        if (const char* a = std::getenv("LPF_ALPHA"))
            return std::stod(a);
        return 0.7;
    }();

    const std::string tcol = [](){
        if (const char* v = std::getenv("LPF_TCOL")) return std::string(v);
        return std::string("t");  // <-- converted to std::string
    }();

    const std::string ycol = [](){
        if (const char* v = std::getenv("LPF_YCOL")) return std::string(v);
        return std::string("z");
    }();

    if (!fs::exists(csvPath))
    {
        GTEST_SKIP() << "Input CSV not found at '" << csvPath
                     << "'. Place 'SonarAlt.csv' in " << DATA_DIR << ".";
    }

    CsvSeries s = CsvIO::Load(csvPath, tcol, ycol);
    ASSERT_FALSE(s.y.empty()) << "CSV had no usable rows: " << csvPath;

    LowPassFilter f;
    std::vector<double> yout; yout.reserve(s.y.size());
    for (double x : s.y)
        yout.push_back(f.update(x, alpha));

    // Write output
    fs::path outFile = fs::path(csvPath).parent_path() / LPF_SIM_CSV;
    CsvIO::Write3(outFile.string(), s.t, s.y, yout, "t", "x", "lpf");

    std::cout << "LPF sim output written to: " << fs::absolute(outFile) << "\n";

    // Optional: basic check for smoothing
    const double stdIn  = CsvIO::StdDev(s.y);
    const double stdOut = CsvIO::StdDev(yout);
    EXPECT_LT(stdOut, stdIn) << "Expected std(lpf) < std(raw)";
}