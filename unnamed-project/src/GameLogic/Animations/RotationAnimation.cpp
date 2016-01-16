#include "GameLogic/Animations/RotationAnimation.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
RotationAnimation::RotationAnimation(ObjectBase *object, Scene *state, std::unique_ptr<InterpolationBase> interpolation, const QVector3D &dest) :
    AnimationBase(object, state, std::move(interpolation)),
    m_interpolatedValue(m_interpolation.get(), m_object->getRotation(), dest)
{}

//------------------------------------------------------------------------------
RotationAnimation::~RotationAnimation()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void RotationAnimation::tick(float time)
{
    auto currentValue = m_interpolatedValue.interpolatedValue(time);

    m_object->setRotation(currentValue);

    // TODO notify game about changes!
}

//------------------------------------------------------------------------------
QString RotationAnimation::type()
{
    return QString(traits::animation_name<RotationAnimation>::value());
}
