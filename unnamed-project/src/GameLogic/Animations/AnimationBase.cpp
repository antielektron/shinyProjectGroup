#include "GameLogic/Animations/AnimationBase.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"
#include <cassert>
#include <cmath>

#define PI 3.1415926536f

AnimationBase::AnimationBase(ObjectBase *object, Scene *scene, std::unique_ptr<InterpolationBase> interpolation) :
        m_object(object),
        m_scene(scene),
        m_interpolation(std::move(interpolation))
{}

//------------------------------------------------------------------------------
AnimationBase::~AnimationBase()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool AnimationBase::isFinished()
{
    return m_finished;
}

//------------------------------------------------------------------------------
ObjectBase *AnimationBase::getObject()
{
    return m_object;
}

//------------------------------------------------------------------------------
void AnimationBase::cancelAnimation()
{
    // just set the interpolated value to the end value,
    // update our object and notify listeners, that we're
    // not in an animation anymore

    // TODO update object
    // TODO remove animation!
}
