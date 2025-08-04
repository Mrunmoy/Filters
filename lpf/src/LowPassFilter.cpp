#include "LowPassFilter.hpp"

namespace Filters {
namespace LPF {

double LowPassFilter::update(double x)
{
    if (m_firstRun) {
        m_prevX = x;
        m_firstRun = false;
    }

    double xlpf = m_alpha * m_prevX + (1.0 - m_alpha) * x;
    m_prevX = xlpf;
    return xlpf;
}

double LowPassFilter::update(double x, double alpha)
{
    if (m_firstRun)
    {
        m_prevX = x;
        m_firstRun = false;
    }

    m_prevX = alpha * m_prevX + (1.0 - alpha) * x;
    return m_prevX;
}


void LowPassFilter::reset()
{
    m_prevX = 0.0;
    m_firstRun = true;
}

} // namespace LPF
} // namespace Filters
