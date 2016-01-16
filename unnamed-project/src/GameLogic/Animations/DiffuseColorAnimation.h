#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_DIFFUSE_COLOR_ANIMATION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_DIFFUSE_COLOR_ANIMATION_H

#include "GameLogic/Animations/AnimationBase.h"
#include "GameLogic/Animations/InterpolatedValue.h"
#include "GameLogic/Traits.h"

class Object;

class DiffuseColorAnimation : public AnimationBase
{
public:
    // TODO: allow ObjectGroups
    // NOTE: every Object could have a different initial color!!
    DiffuseColorAnimation(Object *object, Scene *state, std::unique_ptr<InterpolationBase> interpolation, const QVector3D &dest);
    virtual ~DiffuseColorAnimation();

    virtual void tick(float time) override;

    virtual QString type() override;

protected:
    InterpolatedValue<QVector3D> m_interpolatedValue;
    Object *m_object;
};


namespace traits
{
    template <>
    struct animation_name<DiffuseColorAnimation>
    {
        static constexpr const char *value = "Diffuse Color Animation";
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_DIFFUSE_COLOR_ANIMATION_H
