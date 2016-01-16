#include "GameLogic/Event.h"

#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Actions/ActionBase.h"

#include "GameLogic/Factories/PreconditionFactory.h"
#include "GameLogic/Factories/ActionFactory.h"

#include <iostream>

Event::Event()
{
}

Event::Event(const QString &name) : m_name(name)
{
}

Event::Event(GlobalState *state, const QDomElement &domElement)
{
    m_name = domElement.attribute("name");
    m_description = domElement.attribute("description");

    for (auto child = domElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        QString tag = child.tagName();

        if (tag == "Precondition")
        {
            auto precondition = Factory::createPreconditionFromDomElement(state, child);
            m_preconditions.push_back(std::move(precondition));
        }
        else if (tag == "Action")
        {
            auto action = Factory::createActionFromDomElement(state, child);
            m_actions.push_back(std::move(action));
        }
    }
}

Event::~Event()
{
}

void Event::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Event");
    writer.writeAttribute("name", m_name);
    writer.writeAttribute("description", m_description);

    for (auto &condition : m_preconditions)
    {
        condition->writeToXml(writer);
    }

    for (auto &action : m_actions)
    {
        action->writeToXml(writer);
    }

    writer.writeEndElement();
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
    m_name = name;
}

const QString &Event::getName()
{
    return m_name;
}

void Event::setDescription(const QString &description)
{
    m_description = description;
}

const QString &Event::getDescription()
{
    return m_description;
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
    return createRange(m_preconditions.begin(), m_preconditions.end());
}

range<Event::ActionIterator> Event::getActions()
{
    return createRange(m_actions.begin(), m_actions.end());
}
