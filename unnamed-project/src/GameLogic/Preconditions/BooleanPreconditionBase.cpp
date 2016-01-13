#include "GameLogic/Preconditions/BooleanPreconditionBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
BooleanPreconditionBase::BooleanPreconditionBase(GlobalState *state) :
        PreconditionBase(state)
{}

//------------------------------------------------------------------------------
void BooleanPreconditionBase::addCondition(std::unique_ptr<PreconditionBase> condition)
{
    m_conditions.push_back(std::move(condition));
}

//------------------------------------------------------------------------------
void BooleanPreconditionBase::removeCondition(size_t i)
{
    m_conditions.erase(m_conditions.begin()+i);
}
