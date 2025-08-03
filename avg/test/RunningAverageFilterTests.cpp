#include <gtest/gtest.h>
#include "RunningAverageFilter.hpp"
#include <vector>
#include <numeric>
#include <cmath>
#include <random>
#include <fstream>
#include <iomanip>

using Filters::Avg::RunningAverageFilter;

TEST(RunningAverageFilter, FirstSampleEqualsInput)
{
    RunningAverageFilter f;
    double y = f.update(10.0);
    EXPECT_DOUBLE_EQ(y, 10.0);
    EXPECT_DOUBLE_EQ(f.getAverage(), 10.0);
    EXPECT_EQ(f.getCount(), 1u);
}

TEST(RunningAverageFilter, ComputesArithmeticMean)
{
    RunningAverageFilter f;
    std::vector<double> samples = {10.0, 12.0, 11.0, 13.0, 9.0};

    double out = 0.0;
    for (double s : samples)
    {
        out = f.update(s);
    }

    double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
    double mean = sum / static_cast<double>(samples.size());

    EXPECT_NEAR(out, mean, 1e-12);
    EXPECT_NEAR(f.getAverage(), mean, 1e-12);
    EXPECT_EQ(f.getCount(), samples.size());
}

TEST(RunningAverageFilter, ResetRestoresInitialConditions)
{
    RunningAverageFilter f;
    (void)f.update(10.0);
    (void)f.update(20.0);

    f.reset();
    EXPECT_DOUBLE_EQ(f.getAverage(), 0.0);
    EXPECT_EQ(f.getCount(), 0u);

    double y = f.update(5.0);
    EXPECT_DOUBLE_EQ(y, 5.0);
    EXPECT_DOUBLE_EQ(f.getAverage(), 5.0);
    EXPECT_EQ(f.getCount(), 1u);
}

TEST(RunningAverageFilter, LargeSampleSequenceIsStable)
{
    RunningAverageFilter f;
    const int N = 10000;
    for (int i = 1; i <= N; ++i)
    {
        f.update(1.0);
    }
    EXPECT_NEAR(f.getAverage(), 1.0, 1e-12);
    EXPECT_EQ(f.getCount(), static_cast<unsigned>(N));
}


// --- MATLAB: z = 14.4 + (0 + 4*randn) ---
// Deterministic RNG for tests.
static double GetVolt()
{
    static std::mt19937 rng(42);                                   // fixed seed for reproducibility
    static std::normal_distribution<double> noise(0.0, 4.0);       // mean 0, std dev 4
    return 14.4 + noise(rng);                                      // mean 14.4
}

TEST(RunningAverageFilter, VoltageSimulationWithCsv)
{
    const double dt = 0.2;
    const double T  = 10.0;
    const int Nsamples = static_cast<int>(T / dt) + 1; // t = 0:dt:10

    RunningAverageFilter f;

    std::vector<double> Xmsaved;
    std::vector<double> Avgsaved;
    Xmsaved.reserve(Nsamples);
    Avgsaved.reserve(Nsamples);

    // Open CSV (always overwrite)
    std::ofstream csv("avg_sim.csv", std::ios::out | std::ios::trunc);
    ASSERT_TRUE(csv.is_open()) << "Failed to open avg_sim.csv for writing";
    csv << "t,xm,avg\n";
    csv << std::fixed << std::setprecision(10);

    for (int k = 0; k < Nsamples; ++k)
    {
        const double t   = k * dt;
        const double xm  = GetVolt();
        const double avg = f.update(xm);

        Xmsaved.push_back(xm);
        Avgsaved.push_back(avg);

        csv << t << "," << xm << "," << avg << "\n";
    }
    csv.close();

    // Sanity + correctness checks
    const double sum  = std::accumulate(Xmsaved.begin(), Xmsaved.end(), 0.0);
    const double mean = sum / static_cast<double>(Nsamples);
    EXPECT_NEAR(Avgsaved.back(), mean, 1e-12); // algorithmic equality

    const double sigma = 4.0; // from GetVolt noise
    const double se    = sigma / std::sqrt(static_cast<double>(Nsamples));
    EXPECT_NEAR(Avgsaved.back(), 14.4, 3.0 * se); // close to true mean
}