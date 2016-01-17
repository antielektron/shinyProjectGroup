#ifndef UNNAMED_PROJECT_GAME_LOGIC_GLOBAL_STATE_H
#define UNNAMED_PROJECT_GAME_LOGIC_GLOBAL_STATE_H

#include <memory>

#include <map>
#include <vector>
#include <QString>
#include <QVariant>
#include <QVector3D>

#include "smartiterator.h"
#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Actions/ActionBase.h"

class Event;
class Scene;

class GlobalState
{
public:
    GlobalState(Scene *scene);
    ~GlobalState();

    // clear?
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

    typedef std::map<QString, QVariant>::iterator AttributeIterator;

    range<AttributeIterator> getAttributes();


    typedef std::vector<std::unique_ptr<Event>>::iterator EventIterator;

    void addEvent(std::unique_ptr<Event> event);
    void removeEvent(EventIterator iterator);

    range<EventIterator> getEvents();

    void triggerEvent(const QString &name);

    // TODO remove, ugly workaround!!!!!!!!!!!!!!!
    void setTime(float time);
    float getTime();

protected:
    void initializeExternalAttributes();

    Scene *m_scene;

    float m_time;

    std::map<QString, QVariant> m_attributes;

    std::vector<std::unique_ptr<Event>> m_events;

    // stash:
    std::map<QString, QVariant> m_stashedAttributes;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_GLOBAL_STATE_H
