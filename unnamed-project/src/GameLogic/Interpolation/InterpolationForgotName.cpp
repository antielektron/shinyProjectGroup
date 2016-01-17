#include "GameLogic/Interpolation/InterpolationForgotName.h"

InterpolationForgotName::InterpolationForgotName(double startTime, double endTime) :
        m_startTime(startTime),
        m_endTime(endTime)
{}

InterpolationForgotName::~InterpolationForgotName()
{}

double InterpolationForgotName::evaluate(double time)
{
    if (time > m_endTime)
        return 1.;
    else if (time < m_startTime)
        return 0.;

    double x = (time - m_startTime) / (m_endTime - m_startTime);

    return (3. - 2. * x) * x * x;
}

double InterpolationForgotName::evaluateGradient(double time)
{
    if (time > m_endTime || time < m_startTime)
        return 0.;

    double x = (time - m_startTime) / (m_endTime - m_startTime);
    double dx = 1. / (m_endTime - m_startTime);

    return dx * (3. - 3. * x) * 2 * x;
}

bool InterpolationForgotName::isFinished(double time)
{
    return time >= m_endTime;
}
