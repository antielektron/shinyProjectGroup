#include "GameLogic/Animations/PositionAnimation.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
PositionAnimation::PositionAnimation(ObjectBase *object, Scene *state, std::unique_ptr<InterpolationBase> interpolation, const QVector3D &dest) :
        AnimationBase(object, state, std::move(interpolation)),
        m_interpolatedValue(m_interpolation.get(), m_object->getPosition(), dest)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
PositionAnimation::~PositionAnimation()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void PositionAnimation::tick(float time)
{
    auto currentValue = m_interpolatedValue.interpolatedValue(time);

    m_object->setPosition(currentValue);

    // TODO notify game about changes!
}

//------------------------------------------------------------------------------
QString PositionAnimation::type()
{
    return QString(traits::animation_name<PositionAnimation>::value());
}
