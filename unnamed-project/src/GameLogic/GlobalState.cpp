#include "GameLogic/GlobalState.h"
#include "GameLogic/Animator.h"

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
    initializeConstantAttributes();
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

    if (type == AttributeDatatype::QVector3D)
    {
        notifyListeners(key);
    }
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

//------------------------------------------------------------------------------
void GlobalState::registerAnimator(const QString &watchedAttrib, Animator *anim)
{
    m_animatorsPerAttribute[watchedAttrib].push_back(anim);
}

//------------------------------------------------------------------------------
void GlobalState::initializeConstantAttributes()
{
    setValue(KEY_ATTRIBUTE_TIME, QVariant(0.0f), AttributeDatatype::Float);
    setValue(KEY_ATTRIBUTE_TRUE, QVariant(true), AttributeDatatype::Bool);
    setValue(KEY_ATTRIBUTE_FALSE, QVariant(false), AttributeDatatype::Bool);

    QVector3D playerPosition(0,0,0);
    setValue(KEY_ATTRIBUTE_PLAYER,
             QVariant(playerPosition),
             AttributeDatatype::QVector3D);
}

//------------------------------------------------------------------------------
void GlobalState::notifyListeners(const QString &key)
{
    for (const auto &anim : m_animatorsPerAttribute[key])
    {
        float time = getValue(KEY_ATTRIBUTE_TIME).toFloat();
        QVector3D vec = getValue(key).value<QVector3D>();
        anim->onValueChanged(vec, time);
    }
}
