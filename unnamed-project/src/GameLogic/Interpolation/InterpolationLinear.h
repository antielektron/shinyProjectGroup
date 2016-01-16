#ifndef UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_LINEAR_H
#define UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_LINEAR_H

#include "GameLogic/Interpolation/InterpolationBase.h"

class InterpolationLinear : public InterpolationBase
{
public:
    InterpolationLinear(double startTime, double endTime);
    virtual ~InterpolationLinear();

    virtual double evaluate(double time) override;
    virtual double evaluateGradient(double time) override;

    virtual bool isFinished(double time) override;

private:
    double m_startTime;
    double m_endTime;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_LINEAR_H
