#include "SimpleKalmanFilter.hpp"

namespace Filters
{
namespace Kalman
{

SimpleKalmanFilter::SimpleKalmanFilter()
    : m_a(1.0)
    , m_h(1.0)
    , m_q(0.0)
    , m_r(4.0)
    , m_x(14.0)
    , m_p(6.0)
{
}

double SimpleKalmanFilter::update(double z)
{
    // I. Predict
    const double xp = m_a * m_x;
    const double Pp = m_a * m_p * m_a + m_q;

    // II. Kalman Gain
    const double K = Pp * m_h / (m_h * Pp * m_h + m_r);

    // III. Update estimate
    m_x = xp + K * (z - m_h * xp);

    // IV. Update error covariance
    m_p = Pp - K * m_h * Pp;

    return m_x;
}

} // namespace Kalman
} // namespace Filters
