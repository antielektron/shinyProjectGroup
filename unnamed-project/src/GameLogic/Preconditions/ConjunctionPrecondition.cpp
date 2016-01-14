#include <sstream>

#include "GameLogic/Preconditions/ConjunctionPrecondition.h"
#include "GameLogic/GlobalState.h"
#include "NegationPrecondition.h"

//------------------------------------------------------------------------------
ConjunctionPrecondition::ConjunctionPrecondition(GlobalState *state) :
    BooleanPreconditionBase(state)
{
}

//------------------------------------------------------------------------------
ConjunctionPrecondition::~ConjunctionPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool ConjunctionPrecondition::evaluateCondition()
{
    for (auto &condition : m_conditions)
    {
        if (!condition->evaluateCondition())
            return false;
    }
    return true;
}

//------------------------------------------------------------------------------
QString ConjunctionPrecondition::name()
{
    return traits::precondition_name<ConjunctionPrecondition>::value;
}
