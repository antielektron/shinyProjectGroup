#include "GameLogic/Preconditions/DisjunctionPrecondition.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
DisjunctionPrecondition::DisjunctionPrecondition(GlobalState *state) :
    BooleanPreconditionBase(state)
{
}

//------------------------------------------------------------------------------
DisjunctionPrecondition::~DisjunctionPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool DisjunctionPrecondition::evaluateCondition()
{
    for (auto &condition : m_conditions)
    {
        if (condition->evaluateCondition())
            return true;
    }
    return false;
}

//------------------------------------------------------------------------------
QString DisjunctionPrecondition::toQString()
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
