#include "GameLogic/Preconditions/ConjunctionPrecondition.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
ConjunctionPrecondition::ConjunctionPrecondition(GlobalState *state,
                                                 std::unique_ptr<PreconditionBase> baseA,
                                                 std::unique_ptr<PreconditionBase> baseB) :
    PreconditionBase(state),
    m_baseA(std::move(baseA)),
    m_baseB(std::move(baseB))
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
    return m_baseA->evaluateCondition() && m_baseB->evaluateCondition();
}

//------------------------------------------------------------------------------
QString ConjunctionPrecondition::toQString()
{
    return QString("(") + m_baseA->toQString()
                        + ")&("
                        + m_baseB->toQString()
                        + ")";
}
