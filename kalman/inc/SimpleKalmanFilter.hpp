#pragma once

namespace Filters
{
namespace Kalman
{

class SimpleKalmanFilter
{
public:
    SimpleKalmanFilter();

    // Update with a new measurement
    double update(double z);

private:
    double m_a;  // State transition
    double m_h;  // Measurement model
    double m_q;  // Process noise covariance
    double m_r;  // Measurement noise covariance

    double m_x;  // State estimate
    double m_p;  // Error covariance
};

} // namespace Kalman
} // namespace Filters
