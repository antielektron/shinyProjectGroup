#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_VALUE_ANIMATION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_VALUE_ANIMATION_H

#include "GameLogic/Animations/AnimationBase.h"
#include "GameLogic/Animations/InterpolatedValue.h"
#include "GameLogic/Traits.h"

template <typename T>
class ValueAnimation : public AnimationBase
{
public:
    typedef T AccessType;
    typedef typename AccessType::ValueType ValueType;

    ValueAnimation(Scene *scene, std::unique_ptr<InterpolationBase> interpolation, const AccessType &access, const ValueType &dest);
    virtual ~ValueAnimation() {}

    virtual void tick(float time, IObjectBaseObserver *observer) override;

    virtual QString type() override;

protected:
    AccessType m_access;
    InterpolatedValue<ValueType> m_interpolatedValue;
};


//------------------------------------------------------------------------------
template <typename T>
ValueAnimation<T>::ValueAnimation(Scene *scene, std::unique_ptr<InterpolationBase> interpolation, const AccessType &access, const ValueType &dest) :
        AnimationBase(scene, std::move(interpolation)),
        m_access(access),
        m_interpolatedValue(m_interpolation.get(), m_access.get(), dest)
{}

//------------------------------------------------------------------------------
template <typename T>
void ValueAnimation<T>::tick(float time, IObjectBaseObserver *observer)
{
    auto currentValue = m_interpolatedValue.interpolatedValue(time);

    m_access.set(currentValue);

    if (this->m_interpolation->isFinished(time))
        this->m_scene->deleteAnimation(this);
}

//------------------------------------------------------------------------------
template <typename T>
QString ValueAnimation<T>::type()
{
    // TODO
    return QString();
    // return traits::animation_name<T>::value();
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_OBJECT_ANIMATION_H
