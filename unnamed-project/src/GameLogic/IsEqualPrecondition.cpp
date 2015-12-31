#include "IsEqualPrecondition.h"

//------------------------------------------------------------------------------
template<class T>
IsEqualPrecondition<T>::IsEqualPrecondition(GlobalState *globalState,
                                         const QString &objA,
                                         const QString &objB) :
    BinaryPreconditionBase(globalState, objA, objB)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template<class T>
IsEqualPrecondition<T>::~IsEqualPrecondition()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template<class T>
bool IsEqualPrecondition<T>::evaluateCondition()
{
    const T &a = static_cast<T>(m_globalState->getValue(m_objA));
    const T &b = static_cast<T>(m_globalState->getValue(m_objB));

    return a == b;
}
