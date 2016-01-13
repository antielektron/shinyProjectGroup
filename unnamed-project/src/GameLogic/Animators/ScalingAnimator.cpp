#include "GameLogic/Animators/ScalingAnimator.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
ScalingAnimator::ScalingAnimator(ObjectBase *object,
                                   GlobalState *state,
                                   const QString &attributeKey,
                                   InterpolationType interpolation,
                                   float animationTime) :
    Animator (object, state, attributeKey, interpolation, animationTime)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
ScalingAnimator::~ScalingAnimator()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
AnimationType ScalingAnimator::getAnimationType()
{
    return AnimationType::Scaling;
}

//------------------------------------------------------------------------------
void ScalingAnimator::updateObject()
{
    m_object->setScaling(m_interpolatedValue);
    m_object->updateWorld();
}

//------------------------------------------------------------------------------
const QVector3D &ScalingAnimator::getObjectsValue()
{
    return m_object->getScaling();
}
