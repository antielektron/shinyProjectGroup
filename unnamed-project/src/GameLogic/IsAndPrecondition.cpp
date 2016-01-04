#include "GameLogic/IsAndPrecondition.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
IsAndPrecondition::IsAndPrecondition(GlobalState *state,
                                     std::unique_ptr<PreconditionBase> baseA,
                                     std::unique_ptr<PreconditionBase> baseB) :
    PreconditionBase(state)
{
    m_baseA = std::move(baseA);
    m_baseB = std::move(baseB);
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

//------------------------------------------------------------------------------
QString IsAndPrecondition::toQString()
{
    return QString("(") + m_baseA->toQString()
                        + "&"
                        + m_baseB->toQString()
                        + ")";
}
