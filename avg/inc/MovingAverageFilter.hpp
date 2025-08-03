#pragma once
#include <cstddef>
#include <vector>
#include <limits>
#include <stdexcept>

namespace Filters
{
namespace Avg
{

class MovingAverageFilter
{
public:
    explicit MovingAverageFilter(std::size_t windowSize = 100)
    {
        setWindowSize(windowSize);
        reset();
    }

    // Update with a new sample; returns the current moving average.
    double update(double x);

    // Reset to "first run" state (next update(x) will fill buffer with x).
    void reset();

    // Change window size; resets the filter to first-run state.
    void setWindowSize(std::size_t n)
    {
        if (n == 0) { throw std::invalid_argument("windowSize must be > 0"); }
        m_n = n;
        m_buf.assign(m_n, 0.0);
        m_idx = 0;
        m_sum = 0.0;
        m_initialized = false;
    }

    std::size_t getWindowSize() const { return m_n; }

    // If not initialized yet (no Update called), returns 0.0 by convention.
    double getAverage() const { return (m_initialized ? (m_sum / static_cast<double>(m_n)) : 0.0); }

private:
    std::size_t   m_n{100};
    std::vector<double> m_buf;
    std::size_t   m_idx{0};     // ring index of the element to be replaced next
    double        m_sum{0.0};   // running sum of elements in m_Buf
    bool          m_initialized{false};
};

} // namespace Avg
} // namespace Filters
