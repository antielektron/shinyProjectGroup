#include "GameLogic/Interpolation/InterpolationSin.h"

#define _USE_MATH_DEFINES
#include <algorithm>

InterpolationSin::InterpolationSin(double startTime, double endTime) :
        m_startTime(startTime),
        m_endTime(endTime)
{}

InterpolationSin::~InterpolationSin()
{}

double InterpolationSin::evaluate(double time)
{
    if (time < m_startTime)
        return 0.;
    if (time > m_endTime)
        return 1.;

    double x = (time - m_startTime) / (m_endTime - m_startTime);
    return 0.5 + 0.5 * std::sin((x - 0.5) * M_PI);
}

double InterpolationSin::evaluateGradient(double time)
{
    if (time < m_startTime || time > m_endTime)
        return 0.;

    double x = (time - m_startTime) / (m_endTime - m_startTime);
    double dx = 1. / (m_endTime - m_startTime);

    return 0.5f * M_PI * dx * std::cos((x - 0.5) * M_PI);
}

bool InterpolationSin::isFinished(double time)
{
    return time >= m_endTime;
}
