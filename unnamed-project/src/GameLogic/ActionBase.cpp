#include "GameLogic/ActionBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
ActionBase::ActionBase(GlobalState *globalState)
{
    m_globalState = globalState;
}

//------------------------------------------------------------------------------
ActionBase::~ActionBase()
{
    //nothing to do here
}

//------------------------------------------------------------------------------

