#include "GameLogic/Preconditions/DisjunctionPrecondition.h"

//------------------------------------------------------------------------------
DisjunctionPrecondition::DisjunctionPrecondition()
{
}

//------------------------------------------------------------------------------
DisjunctionPrecondition::DisjunctionPrecondition(const QDomElement &domElement) :
        BooleanPreconditionBase(domElement)
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
