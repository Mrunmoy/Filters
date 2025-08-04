#pragma once

#include <cstdint>
#include <limits>

namespace Filters {
namespace LPF {

class LowPassFilter
{
public:
    explicit LowPassFilter(double alpha = 0.5)
        : m_alpha(alpha), m_prevX(0.0), m_firstRun(true)
    {}

    // Feed one sample; returns filtered output
    double update(double x);
    double update(double x, double alpha);

    void reset();

    void setAlpha(double alpha) { m_alpha = alpha; }
    double getAlpha() const { return m_alpha; }

private:
    double m_alpha;
    double m_prevX;
    bool m_firstRun;
};

} // namespace LPF
} // namespace Filters
