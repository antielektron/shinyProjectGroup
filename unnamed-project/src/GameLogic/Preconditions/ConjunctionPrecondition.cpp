#include <sstream>

#include "GameLogic/Preconditions/ConjunctionPrecondition.h"
#include "GameLogic/GlobalState.h"

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
QString ConjunctionPrecondition::toQString()
{
    QString result;

    for (auto it = m_conditions.begin(); it != m_conditions.end(); it++)
    {
        if (it != m_conditions.begin())
            result += "&";
        result += "(" + (*it)->toQString() + ")";
    }

    return result;
}
