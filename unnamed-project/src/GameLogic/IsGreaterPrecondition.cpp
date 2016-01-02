#include "GameLogic/IsGreaterPrecondition.h"
#include "GameLogic/GlobalState.h"

//------------------------------------------------------------------------------
template<class T>
IsGreaterPrecondition<T>::IsGreaterPrecondition(GlobalState *globalState,
                                         const QString &objA,
                                         const QString &objB) :
    BinaryPreconditionBase(globalState, objA, objB)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template<class T>
IsGreaterPrecondition<T>::~IsGreaterPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template<class T>
bool IsGreaterPrecondition<T>::evaluateCondition()
{
    const T &a = static_cast<T>(m_globalState->getValue(m_objA));
    const T &b = static_cast<T>(m_globalState->getValue(m_objB));

    return a > b;
}
