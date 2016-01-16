#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_SCALING_ANIMATION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_SCALING_ANIMATION_H

#include "GameLogic/Animations/AnimationBase.h"
#include "GameLogic/Animations/InterpolatedValue.h"
#include "GameLogic/Traits.h"

/* using ScalingAnimator is dangerous, as bullet does not allow scaling of objects!! */

class ScalingAnimation : public AnimationBase
{
public:
    ScalingAnimation(ObjectBase *object, Scene *state, std::unique_ptr<InterpolationBase> interpolation, const QVector3D &dest);
    virtual ~ScalingAnimation();

    virtual void tick(float time) override;

    virtual QString type() override;

protected:
    InterpolatedValue<QVector3D> m_interpolatedValue;
};


namespace traits
{
    template <>
    struct animation_name<ScalingAnimation>
    {
        static const char *value() { return "Scaling Animation"; }
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_SCALING_ANIMATION_H
