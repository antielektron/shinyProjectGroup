#include "GameLogic/Preconditions/DisjunctionPrecondition.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
DisjunctionPrecondition::DisjunctionPrecondition(GlobalState *state,
                                                   std::unique_ptr<PreconditionBase> baseA,
                                                   std::unique_ptr<PreconditionBase> baseB) :
    PreconditionBase(state),
    m_baseA(std::move(baseA)),
    m_baseB(std::move(baseB))
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
    return m_baseA->evaluateCondition() || m_baseB->evaluateCondition();
}

//------------------------------------------------------------------------------
QString DisjunctionPrecondition::toQString()
{
    return QString("(") + m_baseA->toQString()
                        + ")|("
                        + m_baseB->toQString()
                        + ")";
}
