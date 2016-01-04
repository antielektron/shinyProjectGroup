#include "GameLogic/RotationAnimator.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
RotationAnimator::RotationAnimator(ObjectBase *object,
                                   GlobalState *state,
                                   const QString &attributeKey,
                                   InterpolationType interpolation,
                                   float animationTime) :
    Animator (object, state, attributeKey, interpolation, animationTime)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
RotationAnimator::~RotationAnimator()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
AnimationType RotationAnimator::getAnimationType()
{
    return AnimationType::Rotation;
}

//------------------------------------------------------------------------------
void RotationAnimator::updateObject()
{
    m_object->setRotation(m_interpolatedValue);
    m_object->updateWorld();
}

//------------------------------------------------------------------------------
const QVector3D &RotationAnimator::getObjectsValue()
{
    return m_object->getRotation();
}
