#include "GameLogic/Animations/ScalingAnimation.h"
#include "Scene/ObjectBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
ScalingAnimation::ScalingAnimation(ObjectBase *object, Scene *state, std::unique_ptr<InterpolationBase> interpolation, const QVector3D &dest) :
        AnimationBase(object, state, std::move(interpolation)),
        m_interpolatedValue(m_interpolation.get(), m_object->getScaling(), dest)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
ScalingAnimation::~ScalingAnimation()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void ScalingAnimation::tick(float time)
{
    auto currentValue = m_interpolatedValue.interpolatedValue(time);

    m_object->setScaling(currentValue);

    // TODO notify game about changes!
}

//------------------------------------------------------------------------------
QString ScalingAnimation::type()
{
    return QString(traits::animation_name<ScalingAnimation>::value());
}
