#include "GameLogic/Preconditions/NegationPrecondition.h"

NegationPrecondition::NegationPrecondition(std::unique_ptr<PreconditionBase> condition) :
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
