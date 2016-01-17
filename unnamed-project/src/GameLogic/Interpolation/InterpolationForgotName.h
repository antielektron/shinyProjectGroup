#ifndef UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_FORGOT_NAME_H
#define UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_FORGOT_NAME_H

#include "GameLogic/Interpolation/InterpolationBase.h"

class InterpolationForgotName : public InterpolationBase
{
public:
    InterpolationForgotName(double startTime, double endTime);
    virtual ~InterpolationForgotName();

    virtual double evaluate(double time) override;
    virtual double evaluateGradient(double time) override;

    virtual bool isFinished(double time) override;

private:
    double m_startTime;
    double m_endTime;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_INTERPOLATION_FORGOT_NAME_H
