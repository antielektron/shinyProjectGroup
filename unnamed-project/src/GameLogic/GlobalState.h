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

class GlobalState
{
public:
    GlobalState();
    virtual ~GlobalState();

    typedef std::map<QString, QVariant>::iterator AttributesIteratorType;
    typedef std::pair<std::unique_ptr<PreconditionBase>, std::unique_ptr<ActionBase>> EventType;
    typedef std::map<QString, EventType> EventMapType;
    typedef EventMapType::iterator EventMapIteratorType;

    virtual void Init();

    const QVariant &getValue(const QString &key);
    void setValue(const QString &key,
                  QVariant value);

    const EventType &getEvent(const QString &key);
    void setEvent(const QString &key,
                  std::unique_ptr<PreconditionBase> precondition,
                  std::unique_ptr<ActionBase> action);

    range<AttributesIteratorType> getAttributes();
    range<EventMapIteratorType> getEvents();

protected:
    std::map<QString, QVariant> m_attributes;
    EventMapType m_eventMap;
};

#endif // GLOBALSTATE_H
