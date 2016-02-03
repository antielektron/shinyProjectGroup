#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_TRIGGER_ANIMATION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_TRIGGER_ANIMATION_H

#include <QString>

#include "GameLogic/Animations/AnimationBase.h"

class TriggerAnimation : public AnimationBase
{
public:
    TriggerAnimation(Scene *scene, const QString eventName, double tiggerTime);
    virtual ~TriggerAnimation() {}

    virtual void tick(float time, IObjectBaseObserver *observer) override;

private:
    QString m_name;
    double m_time;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_TRIGGER_ANIMATION_H
