#pragma once
#include <cstdint>
#include <limits>

namespace Filters
{
namespace Avg
{

class RunningAverageFilter
{
public:
    RunningAverageFilter()
    {
        reset();
    }

    // Feed one sample; returns updated average
    double update(double x);

    // Reset to initial state
    void reset()
    {
        m_prevAvg = 0.0; // previous average
        m_k = 1;         // sample index starts at 1
    }

    double getAverage() const { return m_prevAvg; }

    std::uint64_t getCount() const
    {
        // Number of samples incorporated so far
        return (m_k > 1) ? (m_k - 1) : 0;
    }

private:
    double m_prevAvg;
    std::uint64_t m_k;
};

} // namespace Avg
} // namespace Filters
