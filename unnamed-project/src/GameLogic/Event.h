#ifndef UNNAMED_PROJECT_GAME_LOGIC_EVENT_H
#define UNNAMED_PROJECT_GAME_LOGIC_EVENT_H

#include <vector>
#include <memory>

#include <QString>
#include <QDomElement>
#include <QXmlStreamWriter>

#include "smartiterator.h"

class PreconditionBase;
class ActionBase;

class Event
{
public:
    // default -> empty event.
    Event();
    Event(const QString &name);
    Event(const QDomElement &domElement);
    ~Event();

    void writeToXml(QXmlStreamWriter &writer);

    // TODO evaluate logic
    void triger();

    void setName(const QString &name);
    const QString &getName();

    typedef std::vector<std::unique_ptr<PreconditionBase>>::iterator PreconditionIterator;

    void addPrecondition(std::unique_ptr<PreconditionBase> condition);
    void removePrecondition(PreconditionIterator iterator);

    range<PreconditionIterator> getPreconditions();

    typedef std::vector<std::unique_ptr<ActionBase>>::iterator ActionIterator;

    void addAction(std::unique_ptr<ActionBase> action);
    void removeAction(ActionIterator iterator);

    range<ActionIterator> getActions();

private:
    QString m_eventName;
    std::vector<std::unique_ptr<PreconditionBase>> m_preconditions;

    // ORDERED list
    std::vector<std::unique_ptr<ActionBase>> m_actions;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_EVENT_H
