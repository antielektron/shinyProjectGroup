#ifndef UNNAMED_PROJECT_POSITIONANIMATOR_H
#define UNNAMED_PROJECT_POSITIONANIMATOR_H

#include "GameLogic/Animator.h"

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

#endif // UNNAMED_PROJECT_POSITIONANIMATOR_H
