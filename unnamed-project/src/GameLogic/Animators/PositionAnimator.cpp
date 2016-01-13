#include "GameLogic/Animators/PositionAnimator.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
PositionAnimator::PositionAnimator(ObjectBase *object,
                                   GlobalState *state,
                                   const QString &attributeKey,
                                   InterpolationType interpolation,
                                   float animationTime) :
    Animator (object, state, attributeKey, interpolation, animationTime)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
PositionAnimator::~PositionAnimator()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
AnimationType PositionAnimator::getAnimationType()
{
    return AnimationType::Position;
}

//------------------------------------------------------------------------------
void PositionAnimator::updateObject()
{
    m_object->setPosition(m_interpolatedValue);
    m_object->updateWorld();
}

//------------------------------------------------------------------------------
const QVector3D &PositionAnimator::getObjectsValue()
{
    return m_object->getPosition();
}
