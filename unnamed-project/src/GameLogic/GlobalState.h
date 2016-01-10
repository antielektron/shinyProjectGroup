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

    const QVariant &getValue(const QString &key);
    AttributeDatatype getType(const QString &key);
    void setValue(const QString &key,
                  QVariant value,
                  AttributeDatatype type);

    void removeValue(const QString &key);

    const EventType &getEvent(const QString &key);
    void setEvent(const QString &key,
                  std::unique_ptr<PreconditionBase> precondition,
                  std::unique_ptr<ActionBase> action);

    void removeEvent(const QString &eventKey);

    range<AttributesIteratorType> getAttributes();
    range<EventMapIteratorType> getEvents();

protected:
    std::map<QString, QVariant> m_attributes;
    EventMapType m_eventMap;
    DatatypeMapType m_datatypeMap;
};

#endif // GLOBALSTATE_H
