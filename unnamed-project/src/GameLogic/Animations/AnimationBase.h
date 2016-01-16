#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ANIMATION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ANIMATION_BASE_H

/*
 * TODO
 * move animators to Scene/Animators/
 */

#include <memory>

#include <QString>
#include <QVector3D>
#include <QXmlStreamWriter>
#include <QDomElement>

#include "GameLogic/Interpolation/InterpolationBase.h"

class ObjectBase;
class Scene;
class IObjectBaseObserver;

class AnimationBase
{
public:
    // TODO consider constructor with domElement
    // AnimatorBase(Scene *scene, const QDomElement &domElement);
    AnimationBase(Scene *scene, std::unique_ptr<InterpolationBase> interpolation);
    virtual ~AnimationBase();

    virtual void tick(float time, IObjectBaseObserver *observer) = 0;

    virtual QString type() = 0;

    // virtual void writeToXml(QXmlStreamWriter &writer) = 0;

    // does this make sense, animation should delete itself when done!
    bool isFinished();

    // TODO reset/instant finish/leave as is
    void cancelAnimation();

protected:
    Scene *m_scene;

    std::unique_ptr<InterpolationBase> m_interpolation;
    bool m_finished;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ANIMATION_BASE_H
