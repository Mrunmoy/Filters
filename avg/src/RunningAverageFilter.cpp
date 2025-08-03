#include "RunningAverageFilter.hpp"

namespace Filters
{
namespace Avg
{

/*
Running arithmetic mean (same as the MATLAB persistent-version): https://drive.google.com/drive/folders/1oHuf9X6Iy3tcf6dRBCJVKTFa-KRoBEkB

Given the k-th sample x_k and running average avg_{k-1}:

    alpha_k = (k - 1) / k
    avg_k   = alpha_k * avg_{k-1} + (1 - alpha_k) * x_k

Initial conditions used in the MATLAB code:

    k starts at 1
    avg_0 = 0
    => On first update: alpha_1 = 0, so avg_1 = x_1

Mapping to code variables:

    x_k       -> input sample (x)
    avg_{k-1} -> m_prevAvg before update
    k         -> m_k
    avg_k     -> m_prevAvg after update
*/

// Feed one sample; returns updated average
double RunningAverageFilter::update(double x)
{
    const double alpha = (m_k > 0)
        ? (static_cast<double>(m_k - 1) / static_cast<double>(m_k))
        : 0.0;

    double avg = alpha * m_prevAvg + (1.0 - alpha) * x;
    m_prevAvg = avg;

    if (m_k < std::numeric_limits<std::uint64_t>::max())
    {
        ++m_k;
    }

    return avg;
}

} // namespace Avg
} // namespace Filters