#include "GameLogic/Animations/AnimationBase.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"
#include <cassert>
#include <cmath>

#define PI 3.1415926536f

AnimationBase::AnimationBase(Scene *scene) :
        m_scene(scene)
{}

//------------------------------------------------------------------------------
AnimationBase::~AnimationBase()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void AnimationBase::cancelAnimation()
{
    // just set the interpolated value to the end value,
    // update our object and notify listeners, that we're
    // not in an animation anymore

    // TODO update object
    // TODO remove animation!
    assert(false);
}
