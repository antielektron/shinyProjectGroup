#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
GlobalState::GlobalState()
{
    Init();
}

//------------------------------------------------------------------------------
GlobalState::~GlobalState()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void GlobalState::Init()
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
