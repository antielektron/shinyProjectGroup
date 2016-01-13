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

//------------------------------------------------------------------------------
template<>bool IsGreaterPrecondition<int>::evaluateCondition()
{
    int a = m_globalState->getValue(m_objA).toInt();
    int b = m_globalState->getValue(m_objB).toInt();

    return a > b;
}

//------------------------------------------------------------------------------
template<>bool IsGreaterPrecondition<float>::evaluateCondition()
{
    float a = m_globalState->getValue(m_objA).toFloat();
    float b = m_globalState->getValue(m_objB).toFloat();

    return a > b;
}

//------------------------------------------------------------------------------
template<class T>
QString IsGreaterPrecondition<T>::toQString()
{
    return QString("") + m_objA + ">" + m_objB;
}

//------------------------------------------------------------------------------
template <class T>
QString IsGreaterPrecondition<T>::getTypeofArgs()
{
    // theoretically it should be impossible to call
    // this, btw
    return QString("unknown");
}

//------------------------------------------------------------------------------
template<> QString IsGreaterPrecondition<int>::getTypeofArgs()
{
    return QString("int");
}

//------------------------------------------------------------------------------
template<> QString IsGreaterPrecondition<float>::getTypeofArgs()
{
    return QString("float");
}


template class IsGreaterPrecondition<int>;
template class IsGreaterPrecondition<float>;
