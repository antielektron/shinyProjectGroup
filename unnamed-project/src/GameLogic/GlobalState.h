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
#include "GameLogic/GameLogicDatatypes.h"

class Animator;
class Event;

class GlobalState
{
public:
    GlobalState();
    ~GlobalState();

    typedef std::map<QString, QVariant>::iterator AttributesIteratorType;
    typedef std::map<QString, AttributeDatatype> DatatypeMapType;

    void init();

    // stash current state, so we can apply it later again
    void stash();

    // apply prevoiusly stashed attributes
    void applyStash();

    const QVariant &getValue(const QString &key);
    bool existValue(const QString &key);
    AttributeDatatype getType(const QString &key);

    /**
     * @brief setValue add or replace a value. changes will only applied
     *        after applyBuffer() is called
     * @param key
     * @param value
     * @param type
     */
    void setValue(const QString &key,
                  QVariant value,
                  AttributeDatatype type);

    void removeValue(const QString &key);

    void applyBuffer();


    typedef std::vector<std::unique_ptr<Event>>::iterator EventIterator;

    void addEvent(std::unique_ptr<Event> event);
    void removeEvent(EventIterator iterator);

    range<EventIterator> getEvents();

    void triggerEvent(const QString &name);

    range<AttributesIteratorType> getAttributes();

    void registerAnimator(const QString &watchedAttrib, Animator *anim);

protected:
    void initializeConstantAttributes();
    void notifyListeners(const QString &key);

    std::map<QString, QVariant> m_attributes;
    std::map<QString, std::vector<Animator *>> m_animatorsPerAttribute;
    DatatypeMapType m_datatypeMap;

    std::vector<std::pair<QString, QVariant>> m_attributesQueue;
    std::vector<std::pair<QString, AttributeDatatype>> m_datatypeQueue;
    std::vector<QString> m_notifierList;

    std::vector<std::unique_ptr<Event>> m_events;

    // stash:
    std::map<QString, QVariant> m_stashedAttributes;
    DatatypeMapType m_stashedDatatypeMap;
};

#endif // GLOBALSTATE_H
