#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_POSITION_ANIMATION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_POSITION_ANIMATION_H

#include "GameLogic/Animations/AnimationBase.h"
#include "GameLogic/Animations/InterpolatedValue.h"
#include "GameLogic/Traits.h"

class PositionAnimation : public AnimationBase
{
public:
    PositionAnimation(ObjectBase *object, Scene *state, std::unique_ptr<InterpolationBase> interpolation, const QVector3D &dest);
    virtual ~PositionAnimation();

    virtual void tick(float time) override;

    virtual QString type() override;

protected:
    InterpolatedValue<QVector3D> m_interpolatedValue;
};


namespace traits
{
    template <>
    struct animation_name<PositionAnimation>
    {
        static constexpr const char *value = "Position Animation";
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_POSITION_ANIMATION_H
