#include "GameLogic/IsNotPrecondition.h"
#include "GameLogic/GlobalState.h"

IsNotPrecondition::IsNotPrecondition(GlobalState *state,
                                     PreconditionBase *condition) :
    PreconditionBase (state)
{
    m_condition = condition;
}

//------------------------------------------------------------------------------
IsNotPrecondition::~IsNotPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool IsNotPrecondition::evaluateCondition()
{
    return !m_condition->evaluateCondition();
}
