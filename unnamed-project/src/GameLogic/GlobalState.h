#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include <map>
#include <vector>
#include <QString>
#include <QVariant>
#include <memory>
#include "smartiterator.h"
#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Actions/ActionBase.h"

class Animator;
class Event;
class Scene;

class GlobalState
{
public:
    GlobalState(Scene *scene);
    ~GlobalState();

    void init();

    Scene *getScene();

    // stash current state, so we can apply it later again
    void stash();

    // apply prevoiusly stashed attributes
    void applyStash();

    const QVariant &getValue(const QString &key);
    bool existValue(const QString &key);
    void setValue(const QString &key, const QVariant &value);
    void removeValue(const QString &key);

    typedef std::map<QString, QVariant>::iterator AttributesIterator;

    range<AttributesIterator> getAttributes();

    void registerAnimator(const QString &watchedAttrib, Animator *anim);


    typedef std::vector<std::unique_ptr<Event>>::iterator EventIterator;

    void addEvent(std::unique_ptr<Event> event);
    void removeEvent(EventIterator iterator);

    range<EventIterator> getEvents();

    void triggerEvent(const QString &name);

protected:
    void initializeConstantAttributes();
    void notifyListeners(const QString &key);

    Scene *m_scene;

    std::map<QString, QVariant> m_attributes;
    std::map<QString, std::vector<Animator *>> m_animatorsPerAttribute;

    std::vector<QString> m_notifierList;

    std::vector<std::unique_ptr<Event>> m_events;

    // stash:
    std::map<QString, QVariant> m_stashedAttributes;
};

#endif // GLOBALSTATE_H
