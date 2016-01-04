#include "GameLogic/Animator.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"
#include <cassert>
#include <cmath>

Animator::Animator(ObjectBase *object,
                   GlobalState *state,
                   const QString &attributeKey,
                   InterpolationType interpolation,
                   float animationTime)
{
    m_object = object;
    m_state = state;
    m_isAnimationRunning = false;
    m_animationTime = animationTime;
    m_interpolationType = interpolation;
    // TODO register at globalState
}

//------------------------------------------------------------------------------
Animator::~Animator()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
ObjectBase *Animator::getObject()
{
    return m_object;
}

//------------------------------------------------------------------------------
void Animator::tick(float currentTime)
{
    assert(m_isAnimationRunning);

    float dt = currentTime - m_animationStartTime;
    if (dt >= m_animationTime)
    {
        instantlyFinishCurrentAnimation();
        return;
    }

    switch (m_interpolationType) {
    case InterpolationType::Jump:
    {
        m_interpolatedValue = m_animationTime * 0.5 + m_animationStartTime > currentTime
                              ? m_endValue : m_startValue;
        break;
    }
    case InterpolationType::Linear:
    {
        m_interpolatedValue = (dt / m_animationTime) *
                              (m_endValue - m_startValue) +
                              m_startValue;
        break;
    }
    case InterpolationType::Trigonometric:
    {
        // just using the sinus for interpolating.

        // we have to map our animation Time to the range [0:pi/2]
        float scale = 0.5f * 3.1415926536f / m_animationTime;
        m_interpolatedValue = std::sin(scale * dt) *
                              (m_endValue - m_startValue) +
                              m_startValue;
        break;
    }
    }

    updateObject();

}

//------------------------------------------------------------------------------
void Animator::instantlyFinishCurrentAnimation()
{
    // just set the interpolated value to the end value,
    // update our object and notify listeners, that we're
    // not in an animation anymore

    m_interpolatedValue = m_endValue;
    updateObject();

    m_isAnimationRunning = false;
    emit animationFinished(this);
}

//------------------------------------------------------------------------------
void Animator::onValueChanged(const QVector3D &val, float currentTime)
{

    if (m_isAnimationRunning)
    {
        instantlyFinishCurrentAnimation(); //hmmmm....
    }

    // get start value from the object
    m_startValue = getObjectsValue();
    m_endValue = val;

    m_animationStartTime = currentTime;
    m_animationEndTime = currentTime + m_animationTime;

    m_isAnimationRunning = true;
    emit animationStarted(this);

}

//------------------------------------------------------------------------------

