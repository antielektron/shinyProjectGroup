#include "GameLogic/IsNotPrecondition.h"
#include "GameLogic/GlobalState.h"

IsNotPrecondition::IsNotPrecondition(GlobalState *state,
                                     std::unique_ptr<PreconditionBase> condition) :
    PreconditionBase (state)
{
    m_condition = std::move(condition);
}

//------------------------------------------------------------------------------
IsNotPrecondition::~IsNotPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
bool IsNotPrecondition::evaluateCondition()
{
    return !m_condition->evaluateCondition();
}

//------------------------------------------------------------------------------
QString IsNotPrecondition::toQString()
{
    return QString("(!") + m_condition->toQString() + ")";
}
