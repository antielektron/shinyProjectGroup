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
class GlobalState;

class Event
{
public:
    // default -> empty event.
    Event();
    Event(const QString &name);
    Event(GlobalState *state, const QDomElement &domElement);
    ~Event();

    void writeToXml(QXmlStreamWriter &writer);

    // evaluate logic
    bool triger();

    void setName(const QString &name);
    const QString &getName();

    void setDescription(const QString &description);
    const QString &getDescription();

    typedef std::vector<std::unique_ptr<PreconditionBase>>::iterator PreconditionIterator;

    void addPrecondition(std::unique_ptr<PreconditionBase> condition);
    void removePrecondition(PreconditionIterator iterator);

    range<PreconditionIterator> getPreconditions();

    typedef std::vector<std::unique_ptr<ActionBase>>::iterator ActionIterator;

    void addAction(std::unique_ptr<ActionBase> action);
    void removeAction(ActionIterator iterator);

    range<ActionIterator> getActions();

private:
    QString m_name;
    QString m_description;
    std::vector<std::unique_ptr<PreconditionBase>> m_preconditions;

    // ORDERED list
    std::vector<std::unique_ptr<ActionBase>> m_actions;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_EVENT_H
