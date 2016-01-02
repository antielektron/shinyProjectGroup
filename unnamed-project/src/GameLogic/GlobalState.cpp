#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
GlobalState::GlobalState()
{

}

//------------------------------------------------------------------------------
GlobalState::~GlobalState()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
const QVariant &GlobalState::getValue(const QString &key)
{
    // TODO: maybe check if is Valid?
    return m_attributes[key];
}

//------------------------------------------------------------------------------
void GlobalState::setValue(const QString &key,
                           QVariant value)
{
    m_attributes[key] = value;
}
