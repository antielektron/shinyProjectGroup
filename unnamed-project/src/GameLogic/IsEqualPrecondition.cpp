#include "GameLogic/IsEqualPrecondition.h"
#include "GameLogic/GlobalState.h"

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

//------------------------------------------------------------------------------
template<>bool IsEqualPrecondition<int>::evaluateCondition()
{
    int a = m_globalState->getValue(m_objA).toInt();
    int b = m_globalState->getValue(m_objB).toInt();

    return a == b;
}

//------------------------------------------------------------------------------
template<>bool IsEqualPrecondition<bool>::evaluateCondition()
{
    bool a = m_globalState->getValue(m_objA).toBool();
    bool b = m_globalState->getValue(m_objB).toBool();

    return a == b;
}

//------------------------------------------------------------------------------
template<class T>
QString IsEqualPrecondition<T>::toQString()
{
    return QString("") + m_objA + "=" + m_objB;
}

//------------------------------------------------------------------------------
template <class T>
QString IsEqualPrecondition<T>::getTypeofArgs()
{
    return QString("unknown");
}

//------------------------------------------------------------------------------
template<> QString IsEqualPrecondition<int>::getTypeofArgs()
{
    return QString("int");
}

//------------------------------------------------------------------------------
template<> QString IsEqualPrecondition<bool>::getTypeofArgs()
{
    return QString("bool");
}

template class IsEqualPrecondition<int>;
template class IsEqualPrecondition<bool>;
