#include "BinaryPreconditionBase.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
BinaryPreconditionBase::BinaryPreconditionBase(GlobalState *globalState,
                                               const QString objA,
                                               const QString objB) :
    PreconditionBase (globalState)
{
    m_objA = objA;
    m_objB = objB;
}

//------------------------------------------------------------------------------
BinaryPreconditionBase::~BinaryPreconditionBase()
{
    // nothing to do here...
}
