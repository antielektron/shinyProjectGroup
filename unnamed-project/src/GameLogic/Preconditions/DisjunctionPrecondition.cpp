#include "GameLogic/Preconditions/DisjunctionPrecondition.h"

//------------------------------------------------------------------------------
DisjunctionPrecondition::DisjunctionPrecondition()
{
}

//------------------------------------------------------------------------------
DisjunctionPrecondition::DisjunctionPrecondition(GlobalState *state, const QDomElement &domElement) :
        BooleanPreconditionBase(state, domElement)
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
QString DisjunctionPrecondition::type()
{
    return traits::precondition_name<DisjunctionPrecondition>::value;
}
