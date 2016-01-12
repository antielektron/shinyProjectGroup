#include "IsOrPrecondition.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
IsOrPrecondition::IsOrPrecondition(GlobalState *state,
                                   std::unique_ptr<PreconditionBase> baseA,
                                   std::unique_ptr<PreconditionBase> baseB) :
    PreconditionBase(state)
{
    m_baseA = std::move(baseA);
    m_baseB = std::move(baseB);
}

//------------------------------------------------------------------------------
IsOrPrecondition::~IsOrPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool IsOrPrecondition::evaluateCondition()
{
    return m_baseA->evaluateCondition() || m_baseB->evaluateCondition();
}

//------------------------------------------------------------------------------
QString IsOrPrecondition::toQString()
{
    return QString("(") + m_baseA->toQString()
                        + ")|("
                        + m_baseB->toQString()
                        + ")";
}
