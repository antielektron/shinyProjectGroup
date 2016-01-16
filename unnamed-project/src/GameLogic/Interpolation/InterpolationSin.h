#ifndef UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_SIN_H
#define UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_SIN_H

#include "GameLogic/Interpolation/InterpolationBase.h"

class InterpolationSin : public InterpolationBase
{
public:
    InterpolationSin(double startTime, double endTime);
    virtual ~InterpolationSin();

    virtual double evaluate(double time) override;
    virtual double evaluateGradient(double time) override;

    virtual bool isFinished(double time) override;

private:
    float m_startTime;
    float m_endTime;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_SIN_H
