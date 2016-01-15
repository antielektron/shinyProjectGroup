#include "GameLogic/GlobalState.h"
#include "GameLogic/Animators/Animator.h"
#include "GameLogic/Event.h"

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
bool GlobalState::existValue(const QString &key)
{
    auto it = m_attributes.find(key);
    return it != m_attributes.end();
}

//------------------------------------------------------------------------------
AttributeDatatype GlobalState::getType(const QString &key)
{
    return m_datatypeMap[key];
}

//------------------------------------------------------------------------------
void GlobalState::setValue(const QString &key, QVariant value)
{
    m_attributesQueue.push_back(std::make_pair(key, value));
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
    }
    for (const auto &key : m_notifierList)
    {
        notifyListeners(key);
    }
    m_attributesQueue.clear();
    m_notifierList.clear();
}


//------------------------------------------------------------------------------
void GlobalState::addEvent(std::unique_ptr<Event> event)
{
    m_events.push_back(std::move(event));
    // TODO notify!! (SceneEditorGames responsibility!)
}

//------------------------------------------------------------------------------
void GlobalState::removeEvent(EventIterator iterator)
{
    m_events.erase(iterator);
    // TODO notify!! (critical) (SceneEditorGames responsibility!)
}

//------------------------------------------------------------------------------
range<GlobalState::EventIterator> GlobalState::getEvents()
{
    return createRange(m_events.begin(), m_events.end());
}

//------------------------------------------------------------------------------
void GlobalState::triggerEvent(const QString &name)
{
    // trigger all events with the coresponding name
    for (auto &event : m_events)
    {
        if (event->getName() == name)
        {
            event->triger();
        }
    }
}


//------------------------------------------------------------------------------
range<GlobalState::AttributesIteratorType> GlobalState::getAttributes()
{
    return range<AttributesIteratorType>(m_attributes.begin(),
                                         m_attributes.end());
}

//------------------------------------------------------------------------------
void GlobalState::registerAnimator(const QString &watchedAttrib, Animator *anim)
{
    m_animatorsPerAttribute[watchedAttrib].push_back(anim);
}

//------------------------------------------------------------------------------
void GlobalState::initializeConstantAttributes()
{
    setValue(KEY_ATTRIBUTE_TIME, QVariant(0.0f));
    setValue(KEY_ATTRIBUTE_DELTA_TIME, QVariant(0.0f));
    setValue(KEY_ATTRIBUTE_TRUE, QVariant(true));
    setValue(KEY_ATTRIBUTE_FALSE, QVariant(false));

    /* TODO store coordinates in player_x, player_y, player_z
    QVector3D playerPosition(0,0,0);
    setValue(KEY_ATTRIBUTE_PLAYER,
             QVariant(playerPosition),
             AttributeDatatype::QVector3D);
    */
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
