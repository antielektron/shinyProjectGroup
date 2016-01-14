#include "GameLogic/Preconditions/DisjunctionPrecondition.h"

//------------------------------------------------------------------------------
DisjunctionPrecondition::DisjunctionPrecondition()
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
QString DisjunctionPrecondition::name()
{
    return traits::precondition_name<DisjunctionPrecondition>::value;
}
