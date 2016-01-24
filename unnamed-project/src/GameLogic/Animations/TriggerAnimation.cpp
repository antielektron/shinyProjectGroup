#include "GameLogic/Animations/TriggerAnimation.h"
#include "Scene/Scene.h"

TriggerAnimation::TriggerAnimation(Scene *scene, const QString eventName, double tiggerTime) :
        AnimationBase(scene),
        m_name(eventName),
        m_time(tiggerTime)
{}

void TriggerAnimation::tick(float time, IObjectBaseObserver *observer)
{
    if (time >= m_time)
    {
        // fire!
        m_scene->getGlobalState()->triggerEvent(m_name);

        // and remove
        m_scene->deleteAnimation(this);
    }
}
