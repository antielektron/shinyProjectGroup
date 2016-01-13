#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
PreconditionBase::PreconditionBase(GlobalState *globalState)
{
    m_globalState = globalState;
}

//------------------------------------------------------------------------------
PreconditionBase::~PreconditionBase()
{
    // nothing to do here...
}

