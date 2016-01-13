#include "GameLogic/Actions/InvertBooleanAction.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
InvertBooleanAction::InvertBooleanAction(GlobalState *state, const QString &key) :
    ActionBase(state)
{
    m_key = key;
}

//------------------------------------------------------------------------------
InvertBooleanAction::~InvertBooleanAction()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void InvertBooleanAction::performAction()
{
    bool val =  m_globalState->getValue(m_key).toBool();
    m_globalState->setValue(m_key, !val, AttributeDatatype::Bool);
}

//------------------------------------------------------------------------------
ActionType InvertBooleanAction::getActionType()
{
    return ActionType::FlipBoolean;
}

//------------------------------------------------------------------------------
QString InvertBooleanAction::getDataType()
{
    return QString("bool");
}

//------------------------------------------------------------------------------
const QString &InvertBooleanAction::getKey()
{
    return m_key;
}
