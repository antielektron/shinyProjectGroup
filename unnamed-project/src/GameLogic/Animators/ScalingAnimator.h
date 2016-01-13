#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATORS_SCALING_ANIMATOR_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATORS_SCALING_ANIMATOR_H

#include "GameLogic/Animators/Animator.h"

/* using ScalingAnimator is dangerous, as bullet does not allow scaling of objects!! */

class ScalingAnimator : public Animator
{
public:
    ScalingAnimator(ObjectBase *object,
                     GlobalState *state,
                     const QString &attributeKey,
                     InterpolationType interpolation = InterpolationType::Linear,
                     float animationTime = 1.0f);

    virtual ~ScalingAnimator();

    virtual AnimationType getAnimationType() override;

protected:
    virtual void updateObject() override;
    virtual const QVector3D &getObjectsValue() override;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATORS_SCALING_ANIMATOR_H
