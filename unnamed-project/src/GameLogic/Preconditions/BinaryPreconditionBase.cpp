#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
BinaryPreconditionBase::BinaryPreconditionBase(GlobalState *globalState, const QString fieldA, const QString fieldB) :
        PreconditionBase(globalState)
{
    m_fieldA = fieldA;
    m_fieldB = fieldB;
}

//------------------------------------------------------------------------------
BinaryPreconditionBase::~BinaryPreconditionBase()
{
    // nothing to do here...
}
