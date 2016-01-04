#ifndef UNNAMED_PROJECT_ROTATIONANIMATOR_H
#define UNNAMED_PROJECT_ROTATIONANIMATOR_H

#include "GameLogic/Animator.h"


class RotationAnimator : public Animator
{
public:
    RotationAnimator(ObjectBase *object,
                     GlobalState *state,
                     const QString &attributeKey,
                     InterpolationType interpolation = InterpolationType::Linear,
                     float animationTime = 1.0f);

    virtual ~RotationAnimator();

    virtual AnimationType getAnimationType() override;

protected:
    virtual void updateObject() override;
    virtual const QVector3D &getObjectsValue() override;
};

#endif // UNNAMED_PROJECT_ROTATIONANIMATOR_H
