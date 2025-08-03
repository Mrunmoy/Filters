#include "MovingAverageFilter.hpp"


/*
Moving Average (fixed window, MATLAB-compatible initialization) : https://drive.google.com/drive/folders/1oJkDBsuNRK-pCmI6lTG5O2f0DuqpBGG4

MATLAB reference:

    n = 100;
    xbuf = x * ones(n,1);   % on first call only
    % then each call:
    xbuf(1:n-1) = xbuf(2:n);
    xbuf(n) = x;
    avg = sum(xbuf)/n;

Efficient O(1) C++ formulation (ring buffer + running sum):

    old = buf[idx];
    sum = sum + x - old;
    buf[idx] = x;
    idx = (idx + 1) % n;
    avg = sum / n;

NOTE: To match MATLAB’s "firstRun" behavior, on the very first Update(x) we
      fill the entire buffer with x so the first avg equals x.
*/

namespace Filters
{
namespace Avg
{

double MovingAverageFilter::update(double x)
{
    if (!m_initialized)
    {
        // First run: fill the buffer with x (matches MATLAB behavior)
        std::fill(m_buf.begin(), m_buf.end(), x);
        m_sum = static_cast<double>(m_n) * x;
        m_idx = 0;
        m_initialized = true;
        return m_sum / static_cast<double>(m_n);
    }

    // Replace the oldest sample with x, update running sum
    const double old = m_buf[m_idx];
    m_sum += x - old;
    m_buf[m_idx] = x;
    m_idx = (m_idx + 1) % m_n;

    return m_sum / static_cast<double>(m_n);
}

void MovingAverageFilter::reset()
{
    m_sum = 0.0;
    m_idx = 0;
    m_initialized = false;
    // m_buf is kept allocated at size m_n; contents will be filled on first update
}


} // namespace Avg
} // namespace Filters
