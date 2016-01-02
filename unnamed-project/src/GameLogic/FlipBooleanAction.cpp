
#include "GameLogic/FlipBooleanAction.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
FlipBooleanAction::FlipBooleanAction(GlobalState *state,
                                     const QString &key) :
    ActionBase (state)
{
    m_key = key;
}

//------------------------------------------------------------------------------
FlipBooleanAction::~FlipBooleanAction()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void FlipBooleanAction::performAction()
{
    bool val =  m_globalState->getValue(m_key).toBool();
    m_globalState->setValue(m_key, !val);
}
