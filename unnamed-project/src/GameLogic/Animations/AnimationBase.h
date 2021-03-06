#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ANIMATION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ANIMATION_BASE_H

/*
 * TODO
 * move animators to Scene/Animators/
 */

class Scene;
class IObjectBaseObserver;

class AnimationBase
{
public:
    // TODO consider constructor with domElement
    // AnimatorBase(Scene *scene, const QDomElement &domElement);
    AnimationBase(Scene *scene);
    virtual ~AnimationBase();

    virtual void tick(float time, IObjectBaseObserver *observer) = 0;

    // virtual void writeToXml(QXmlStreamWriter &writer) = 0;

    // TODO reset/instant finish/leave as is
    void cancelAnimation();

protected:
    Scene *m_scene;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ANIMATION_BASE_H
