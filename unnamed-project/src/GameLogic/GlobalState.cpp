#include "GameLogic/GlobalState.h"
#include "GameLogic/Event.h"

#include <iostream>

//------------------------------------------------------------------------------
GlobalState::GlobalState(Scene *scene) :
        m_scene(scene)
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
    initializeExternalAttributes();
}

//------------------------------------------------------------------------------
Scene *GlobalState::getScene()
{
    return m_scene;
}

//------------------------------------------------------------------------------
void GlobalState::stash()
{
    // copy everything to stash
    m_stashedAttributes = m_attributes;
}

//------------------------------------------------------------------------------
void GlobalState::applyStash()
{
    // copy back (TODO move back?)
    m_attributes = m_stashedAttributes;
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
void GlobalState::setValue(const QString &key, const QVariant &value)
{
    m_attributes[key] = value;
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
void GlobalState::addEvent(std::unique_ptr<Event> event)
{
    m_events.push_back(std::move(event));
}

//------------------------------------------------------------------------------
void GlobalState::removeEvent(EventIterator iterator)
{
    m_events.erase(iterator);
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
range<GlobalState::AttributeIterator> GlobalState::getAttributes()
{
    return range<AttributeIterator>(m_attributes.begin(), m_attributes.end());
}

//------------------------------------------------------------------------------
void GlobalState::initializeExternalAttributes()
{
    // TODO check what we need!
    // setValue(KEY_ATTRIBUTE_TIME, QVariant(0.0f));

    // TODO store coordinates in player_x, player_y, player_z (maybe, maybe not!)
    // QVector3D playerPosition(0, 0, 0);
    // setValue(KEY_ATTRIBUTE_PLAYER, QVariant(playerPosition));
}
