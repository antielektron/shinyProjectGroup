#include "GameLogic/Event.h"

#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Actions/ActionBase.h"

Event::Event()
{
}

Event::Event(const QString &name) : m_eventName(name)
{
}

Event::~Event()
{
}

void Event::triger()
{
    for (auto &condition : m_preconditions)
    {
        if (!condition->evaluateCondition())
        {
            return;
        }
    }

    for (auto &action : m_actions)
    {
        action->performAction();
    }
}

void Event::setName(const QString &name)
{
    m_eventName = name;
}

const QString &Event::getName()
{
    return m_eventName;
}

void Event::addPrecondition(std::unique_ptr<PreconditionBase> condition)
{
    m_preconditions.push_back(std::move(condition));
}

void Event::removePrecondition(PreconditionIterator iterator)
{
    m_preconditions.erase(iterator);
}

void Event::addAction(std::unique_ptr<ActionBase> action)
{
    m_actions.push_back(std::move(action));
}

void Event::removeAction(ActionIterator iterator)
{
    m_actions.erase(iterator);
}

range<Event::PreconditionIterator> Event::getPreconditions()
{
    createRange(m_preconditions.begin(), m_preconditions.end());
}

range<Event::ActionIterator> Event::getActions()
{
    createRange(m_actions.begin(), m_actions.end());
}
