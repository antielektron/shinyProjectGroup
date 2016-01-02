#include "GameLogic/IsAndPrecondition.h"

//------------------------------------------------------------------------------
IsAndPrecondition::IsAndPrecondition(GlobalState *state,
                                     PreconditionBase *baseA,
                                     PreconditionBase *baseB) :
    PreconditionBase(state)
{
    m_baseA = baseA;
    m_baseB = baseB;
}

//------------------------------------------------------------------------------
IsAndPrecondition::~IsAndPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool IsAndPrecondition::evaluateCondition()
{
    return m_baseA->evaluateCondition() && m_baseB->evaluateCondition();
}
