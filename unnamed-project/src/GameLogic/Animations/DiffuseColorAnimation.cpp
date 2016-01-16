#include "GameLogic/Animations/DiffuseColorAnimation.h"
#include "Scene/Object.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
DiffuseColorAnimation::DiffuseColorAnimation(Object *object, Scene *state, std::unique_ptr<InterpolationBase> interpolation, const QVector3D &dest) :
        AnimationBase(object, state, std::move(interpolation)),
        m_object(object),
        m_interpolatedValue(m_interpolation.get(), m_object->getDiffuseColor(), dest)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
DiffuseColorAnimation::~DiffuseColorAnimation()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void DiffuseColorAnimation::tick(float time)
{
    auto currentValue = m_interpolatedValue.interpolatedValue(time);

    m_object->setDiffuseColor(currentValue);

    // TODO notify game about changes!
}

//------------------------------------------------------------------------------
QString DiffuseColorAnimation::type()
{
    return QString(traits::animation_name<DiffuseColorAnimation>::value());
}
