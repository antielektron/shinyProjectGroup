#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_OBJECT_ANIMATION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_OBJECT_ANIMATION_H

#include "GameLogic/Animations/ValueAnimation.h"

class ObjectBase;

template <typename T>
class ObjectAnimation : public ValueAnimation<T>
{
public:
    typedef T AccessType;
    typedef typename AccessType::ValueType ValueType;

    ObjectAnimation(ObjectBase *object, Scene *scene, std::unique_ptr<InterpolationBase> interpolation, const AccessType &access, const ValueType &dest);
    virtual ~ObjectAnimation() {}

    virtual void tick(float time, IObjectBaseObserver *observer) override;

    virtual QString type() override;

protected:
    ObjectBase *m_object;
};


//------------------------------------------------------------------------------
template <typename T>
ObjectAnimation<T>::ObjectAnimation(ObjectBase *object, Scene *scene, std::unique_ptr<InterpolationBase> interpolation, const AccessType &access, const ValueType &dest) :
        ValueAnimation<T>(scene, std::move(interpolation), access, dest),
        m_object(object)
{}

//------------------------------------------------------------------------------
template <typename T>
void ObjectAnimation<T>::tick(float time, IObjectBaseObserver *observer)
{
    auto currentValue = this->m_interpolatedValue.interpolatedValue(time);

    this->m_access.set(currentValue);

    observer->notify(this->m_object);
}

//------------------------------------------------------------------------------
template <typename T>
QString ObjectAnimation<T>::type()
{
    // TODO
    return QString();
    // return traits::animation_name<T>::value();
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_OBJECT_ANIMATION_H
