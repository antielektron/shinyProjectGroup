#include "GameLogic/StateTransition.h"

//------------------------------------------------------------------------------
StateTransition::StateTransition(const std::string &name)
{
    m_eventName = name;
}

//------------------------------------------------------------------------------
StateTransition::~StateTransition()
{
    // nothing to do here
}

//------------------------------------------------------------------------------
const std::string &StateTransition::getName()
{
    return m_eventName;
}

