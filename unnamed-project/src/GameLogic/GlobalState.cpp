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
void GlobalState::stash()
{

    // apply Buffer (just in case)
    applyBuffer();

    // clear stash
    m_stashedAttributes.clear();
    m_stashedDatatypeMap.clear();

    // build stash:
    for (const auto &p : m_attributes)
    {
        m_stashedAttributes[p.first] = p.second;
    }
    for (const auto &p :m_datatypeMap)
    {
        m_stashedDatatypeMap[p.first] = p.second;
    }
}

//------------------------------------------------------------------------------
void GlobalState::applyStash()
{
    // apply buffer (just in case)
    applyBuffer();

    for (const auto &p : m_stashedAttributes)
    {
        m_attributes[p.first] = p.second;
    }
    for (const auto &p : m_stashedDatatypeMap)
    {
        m_datatypeMap[p.first] = p.second;
        // notify animators:
        if (p.second == AttributeDatatype::QVector3D)
        {
            notifyListeners(p.first);
        }
    }
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
    m_attributesQueue.push_back(std::make_pair(key, value));
    m_datatypeQueue.push_back(std::make_pair(key, type));

    if (type == AttributeDatatype::QVector3D)
    {
        m_notifierList.push_back(key);
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
void GlobalState::applyBuffer()
{
    for (size_t i = 0; i < m_attributesQueue.size(); i++)
    {
        m_attributes[m_attributesQueue[i].first] = m_attributesQueue[i].second;
        m_datatypeMap[m_datatypeQueue[i].first] = m_datatypeQueue[i].second;
    }
    for (const auto &key : m_notifierList)
    {
        notifyListeners(key);
    }
    m_attributesQueue.clear();
    m_datatypeQueue.clear();
    m_notifierList.clear();
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
    setValue(KEY_ATTRIBUTE_DELTA_TIME, QVariant(0.0f), AttributeDatatype::Float);
    setValue(KEY_ATTRIBUTE_TRUE, QVariant(true), AttributeDatatype::Bool);
    setValue(KEY_ATTRIBUTE_FALSE, QVariant(false), AttributeDatatype::Bool);

    QVector3D playerPosition(0,0,0);
    setValue(KEY_ATTRIBUTE_PLAYER,
             QVariant(playerPosition),
             AttributeDatatype::QVector3D);

    applyBuffer();
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
