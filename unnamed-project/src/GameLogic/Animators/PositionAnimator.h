#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATORS_POSITION_ANIMATOR_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATORS_POSITION_ANIMATOR_H

#include "GameLogic/Animators/Animator.h"

class PositionAnimator : public Animator
{
public:
    PositionAnimator(ObjectBase *object,
                     GlobalState *state,
                     const QString &attributeKey,
                     InterpolationType interpolation = InterpolationType::Linear,
                     float animationTime = 1.0f);

    virtual ~PositionAnimator();

    virtual AnimationType getAnimationType() override;

protected:
    virtual void updateObject() override;
    virtual const QVector3D &getObjectsValue() override;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATORS_POSITION_ANIMATOR_H
