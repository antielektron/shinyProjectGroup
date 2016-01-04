#include "GameLogic/CopyAttributeAction.h"
#include "GameLogic/GlobalState.h"

CopyAttributeAction::CopyAttributeAction(GlobalState *state,
                                         const QString &src,
                                         const QString &dst) :
    ActionBase (state)
{
    m_srcKey = src;
    m_dstKey = dst;
}

//------------------------------------------------------------------------------
CopyAttributeAction::~CopyAttributeAction()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void CopyAttributeAction::performAction()
{
    QVariant val = m_globalState->getValue(m_srcKey);
    m_globalState->setValue(m_dstKey, val, m_globalState->getType(m_srcKey));
}

//------------------------------------------------------------------------------
ActionType CopyAttributeAction::getActionType()
{
    return ActionType::CopyAttribute;
}

//------------------------------------------------------------------------------
QString CopyAttributeAction::getDataType()
{
    return QString("varying");
}

//------------------------------------------------------------------------------
const QString &CopyAttributeAction::getSourceKey()
{
    return m_srcKey;
}

//------------------------------------------------------------------------------
const QString &CopyAttributeAction::getDestKey()
{
    return m_dstKey;
}
