#include "SimpleKalmanFilter.hpp"
#include "CsvData.hpp"

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <cstdlib>  // for getenv
#include <tuple>

using namespace Filters::Kalman;
namespace fs = std::filesystem;

TEST(SimpleKalmanFilter, SanityAndCSVWrite)
{
    const double defaultR = 4.0;

    // Allow override of measurement noise via env
    const double R = [&]() {
        if (const char* val = std::getenv("KALMAN_R"))
            return std::stod(val);
        return defaultR;
    }();

    SimpleKalmanFilter kf;

    // Inject noise into measurements
    std::vector<double> z, filtered;
    for (int i = 0; i < 100; ++i)
    {
        double noise = ((rand() % 100) / 100.0) - 0.5; // random [-0.5, 0.5]
        double raw = 10.0 + noise;
        z.push_back(raw);
        filtered.push_back(kf.update(raw));
    }

    fs::path outPath = std::string(DATA_DIR) + "/kalman_sanity.csv";
    std::ofstream out(outPath);
    ASSERT_TRUE(out.is_open());

    out << "step,z,filtered\n";
    for (size_t i = 0; i < z.size(); ++i)
        out << i << "," << z[i] << "," << filtered[i] << "\n";

    out.close();
    std::cout << "Kalman sanity test CSV written to: " << outPath << "\n";
}

TEST(SimpleKalmanFilter, SimulationWithVoltage)
{
    const std::string csvPath = std::string(DATA_DIR) + "/Voltage.csv";

    const std::string tcol = []() {
        if (const char* v = std::getenv("KALMAN_TCOL")) return std::string(v);
        return std::string("t");
    }();

    const std::string ycol = []() {
        if (const char* v = std::getenv("KALMAN_YCOL")) return std::string(v);
        return std::string("z");
    }();

    if (!fs::exists(csvPath))
    {
        GTEST_SKIP() << "Voltage.csv not found at '" << csvPath << "'.";
    }

    CsvSeries s = CsvIO::Load(csvPath, tcol, ycol);
    ASSERT_FALSE(s.y.empty()) << "CSV had no rows: " << csvPath;

    SimpleKalmanFilter kf;
    std::vector<std::tuple<double, double, double>> results;

    for (size_t i = 0; i < s.y.size(); ++i)
    {
        double t = (i < s.t.size()) ? s.t[i] : static_cast<double>(i);
        double raw = s.y[i];
        double filtered = kf.update(raw);
        results.emplace_back(t, raw, filtered);
    }

    fs::path outFile = fs::path(csvPath).parent_path() / "kalman_voltage.csv";
    std::ofstream out(outFile);
    ASSERT_TRUE(out.is_open());

    out << "t,z,kalman\n";
    for (const auto& [t, z, k] : results)
        out << t << "," << z << "," << k << "\n";

    out.close();
    std::cout << "Kalman filter output written to: " << outFile << "\n";
}
