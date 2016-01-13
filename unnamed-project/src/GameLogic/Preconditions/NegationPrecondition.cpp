#include "GameLogic/Preconditions/NegationPrecondition.h"
#include "GameLogic/GlobalState.h"

NegationPrecondition::NegationPrecondition(GlobalState *state, std::unique_ptr<PreconditionBase> condition) :
        PreconditionBase(state),
        m_condition(std::move(condition))
{
}

//------------------------------------------------------------------------------
NegationPrecondition::~NegationPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool NegationPrecondition::evaluateCondition()
{
    return !m_condition->evaluateCondition();
}

//------------------------------------------------------------------------------
QString NegationPrecondition::toQString()
{
    return QString("!(") + m_condition->toQString() + ")";
}
