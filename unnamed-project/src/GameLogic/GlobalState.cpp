#include "GameLogic/GlobalState.h"

#include <iostream>

//------------------------------------------------------------------------------
GlobalState::GlobalState()
{
    init();
}

//------------------------------------------------------------------------------
GlobalState::~GlobalState()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void GlobalState::init()
{
    m_attributes.clear();
}

//------------------------------------------------------------------------------
const QVariant &GlobalState::getValue(const QString &key)
{
    // TODO: maybe check if is Valid?
    return m_attributes[key];
}

//------------------------------------------------------------------------------
AttributeDatatype GlobalState::getType(const QString &key)
{
    return m_datatypeMap[key];
}

//------------------------------------------------------------------------------
void GlobalState::setValue(const QString &key,
                           QVariant value,
                           AttributeDatatype type)
{
    m_attributes[key] = value;
    m_datatypeMap[key] = type;
}

//------------------------------------------------------------------------------
void GlobalState::removeValue(const QString &key)
{
    auto it = m_attributes.find(key);
    if (it != m_attributes.end())
    {
        m_attributes.erase(it);
    }
    else
    {
        std::cout << "Warning: could not remove attribute '"
                  << key.toStdString() << "'" << std::endl;
    }
}

//------------------------------------------------------------------------------
const GlobalState::EventType &GlobalState::getEvent(const QString &key)
{
    return m_eventMap[key];
}

//------------------------------------------------------------------------------
void GlobalState::setEvent(const QString &key,
                           std::unique_ptr<PreconditionBase> precondition,
                           std::unique_ptr<ActionBase> action)
{
    m_eventMap[key] = std::make_pair(std::move(precondition),
                                     std::move(action));
}

//------------------------------------------------------------------------------
void GlobalState::removeEvent(const QString &eventKey)
{
    auto it = m_eventMap.find(eventKey);
    if (it != m_eventMap.end())
    {
        m_eventMap.erase(it);
    }
    else
    {
        std::cout << "Warning: could not remove event '"
                  << eventKey.toStdString() << "'" << std::endl;
    }
}

//------------------------------------------------------------------------------
range<GlobalState::AttributesIteratorType> GlobalState::getAttributes()
{
    return range<AttributesIteratorType>(m_attributes.begin(),
                                         m_attributes.end());
}

//------------------------------------------------------------------------------
range<GlobalState::EventMapIteratorType> GlobalState::getEvents()
{
    return range<EventMapIteratorType>(m_eventMap.begin(),
                                       m_eventMap.end());
}
