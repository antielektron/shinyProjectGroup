#include "GameLogic/Interpolation/InterpolationLinear.h"

InterpolationLinear::InterpolationLinear(double startTime, double endTime) :
        m_startTime(startTime),
        m_endTime(endTime)
{}

InterpolationLinear::~InterpolationLinear()
{}

double InterpolationLinear::evaluate(double time)
{
    if (time < m_startTime)
        return 0.;
    if (time > m_endTime)
        return 1.;
    else
        return (time - m_startTime) / (m_endTime - m_startTime);
}

double InterpolationLinear::evaluateGradient(double time)
{
    if (time < m_startTime || time > m_endTime)
        return 0.;
    else
        return 1. / (m_endTime - m_startTime);
}

bool InterpolationLinear::isFinished(double time)
{
    return time >= m_endTime;
}
