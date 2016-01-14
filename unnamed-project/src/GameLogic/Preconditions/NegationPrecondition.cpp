#include "GameLogic/Preconditions/NegationPrecondition.h"
#include "GameLogic/GlobalState.h"
#include "IsLessPrecondition.h"
#include "IsGreaterPrecondition.h"

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
QString NegationPrecondition::string()
{
    return name() + "(" + m_condition->string() + ")";
}

//------------------------------------------------------------------------------
QString NegationPrecondition::name()
{
    return traits::precondition_name<NegationPrecondition>::value;
}
