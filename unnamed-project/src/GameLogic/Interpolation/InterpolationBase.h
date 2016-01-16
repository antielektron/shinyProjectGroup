#ifndef UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_BASE_H

class InterpolationBase
{
public:
    InterpolationBase() {}
    virtual ~InterpolationBase() {}

    /*
     * should return value in [0, 1]
     */
    virtual double evaluate(double time) = 0;

    // use for velocity!!
    virtual double evaluateGradient(double time) = 0;

    // TODO combine with evaluate
    virtual bool isFinished(double time) = 0;

    // TODO evaluate wether we have to write them or not, as the will be created on the fly!?
    virtual void writeToXml() = 0;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_BASE_H
