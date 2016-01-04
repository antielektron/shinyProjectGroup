#ifndef UNNAMED_PROJECT_SCALINGANIMATOR_H
#define UNNAMED_PROJECT_SCALINGANIMATOR_H

#include "GameLogic/Animator.h"

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

#endif // UNNAMED_PROJECT_SCALINGANIMATOR_H
