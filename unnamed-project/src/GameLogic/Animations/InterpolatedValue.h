#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_INTERPOLATED_VALUE_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_INTERPOLATED_VALUE_H

#include "GameLogic/Interpolation/InterpolationBase.h"

template <typename T>
class InterpolatedValue
{
public:
    InterpolatedValue(InterpolationBase *interpolation, const T &source, const T &dest);
    ~InterpolatedValue() {}

    T interpolatedValue(float time);

private:
    InterpolationBase *m_interpolation;

    T m_source;
    T m_dest;
};


//------------------------------------------------------------------------------
template <typename T>
InterpolatedValue<T>::InterpolatedValue(InterpolationBase *interpolation, const T &source, const T &dest) :
        m_interpolation(interpolation),
        m_source(source),
        m_dest(dest)
{}

//------------------------------------------------------------------------------
template <typename T>
T InterpolatedValue<T>::interpolatedValue(float time)
{
    double lambda = m_interpolation->evaluate(time);

    return lambda*m_dest + (1.-lambda)*m_source;
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_INTERPOLATED_VALUE_H
