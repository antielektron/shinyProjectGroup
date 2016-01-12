#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include <map>
#include <vector>
#include <QString>
#include <QVariant>
#include <memory>
#include "smartiterator.h"
#include "GameLogic/PreconditionBase.h"
#include "GameLogic/ActionBase.h"
#include "GameLogic/GameLogicDatatypes.h"

class Animator;


class GlobalState
{
public:

    GlobalState();
    ~GlobalState();

    typedef std::map<QString, QVariant>::iterator AttributesIteratorType;
    typedef std::pair<std::unique_ptr<PreconditionBase>, std::unique_ptr<ActionBase>> EventType;
    typedef std::map<QString, EventType> EventMapType;
    typedef EventMapType::iterator EventMapIteratorType;
    typedef std::map<QString, AttributeDatatype> DatatypeMapType;

    void init();

    // stash current state, so we can apply it later again
    void stash();

    // apply prevoiusly stashed attributes
    void applyStash();

    const QVariant &getValue(const QString &key);
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

    const EventType &getEvent(const QString &key);
    void setEvent(const QString &key,
                  std::unique_ptr<PreconditionBase> precondition,
                  std::unique_ptr<ActionBase> action);

    void removeEvent(const QString &eventKey);

    range<AttributesIteratorType> getAttributes();
    range<EventMapIteratorType> getEvents();

    void registerAnimator(const QString &watchedAttrib, Animator *anim);

protected:
    void initializeConstantAttributes();
    void notifyListeners(const QString &key);

    std::map<QString, QVariant> m_attributes;
    std::map<QString, std::vector<Animator *>> m_animatorsPerAttribute;
    EventMapType m_eventMap;
    DatatypeMapType m_datatypeMap;

    std::vector<std::pair<QString, QVariant>> m_attributesQueue;
    std::vector<std::pair<QString, AttributeDatatype>> m_datatypeQueue;
    std::vector<QString> m_notifierList;

    // stash:
    std::map<QString, QVariant> m_stashedAttributes;
    DatatypeMapType m_stashedDatatypeMap;
};

#endif // GLOBALSTATE_H
