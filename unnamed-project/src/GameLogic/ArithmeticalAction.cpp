#include "GameLogic/ArithmeticalAction.h"
#include "GameLogic/GlobalState.h"

template<class T>
ArithmeticalAction<T>::ArithmeticalAction(GlobalState *state,
                                       const QString &valA,
                                       const QString &valB,
                                       const QString &valDst,
                                       ArithmeticalOperationType type) :
    ActionBase (state)
{
    m_valA = valA;
    m_valB = valB;
    m_valDst = valDst;

    m_type = type;
}

//------------------------------------------------------------------------------
template<class T>
ArithmeticalAction<T>::~ArithmeticalAction()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template<class T>
void ArithmeticalAction<T>::performAction()
{
    T valA = getValueFromQVariant(m_globalState->getValue(m_valA));
    T valB = getValueFromQVariant(m_globalState->getValue(m_valB));
    T result;
    switch(m_type)
    {
    case ArithmeticalOperationType::additionType :
    {
        result = valA + valB;
        break;
    }
    case ArithmeticalOperationType::subtractionType :
    {
        result = valA - valB;
        break;
    }
    case ArithmeticalOperationType::multiplicationType :
    {
        result = valA * valB;
        break;
    }
    case ArithmeticalOperationType::divisionType :
    {
        result = valA / valB;
        break;
    }
    }

    m_globalState->setValue(m_valDst, result);
}

//------------------------------------------------------------------------------
template<class T>
ActionType ArithmeticalAction<T>::getActionType()
{
    return ActionType::ArithmeticalAction;
}

//------------------------------------------------------------------------------
template<class T>
QString ArithmeticalAction<T>::getDataType()
{
    // just for design reasons. Is never called.
    return QString("unkown");
}

//------------------------------------------------------------------------------
template<>
QString ArithmeticalAction<int>::getDataType()
{
    return QString("int");
}

//------------------------------------------------------------------------------
template<>
QString ArithmeticalAction<float>::getDataType()
{
    return QString("float");
}

//------------------------------------------------------------------------------
template <class T>
ArithmeticalOperationType ArithmeticalAction<T>::getOperationType()
{
    return m_type;
}

//------------------------------------------------------------------------------
template <class T>
const QString &ArithmeticalAction<T>::getLeftOperandKey()
{
    return m_valA;
}

//------------------------------------------------------------------------------
template <class T>
const QString &ArithmeticalAction<T>::getRightOperandKey()
{
    return m_valB;
}

//------------------------------------------------------------------------------
template <class T>
const QString &ArithmeticalAction<T>::getDestKey()
{
    return m_valDst;
}

//------------------------------------------------------------------------------
template <class T>
T ArithmeticalAction<T>::getValueFromQVariant(QVariant v)
{
    return static_cast<T>(v);
}

//------------------------------------------------------------------------------
template<>
int ArithmeticalAction<int>::getValueFromQVariant(QVariant v)
{
    return v.toInt();
}

//------------------------------------------------------------------------------
template<>
float ArithmeticalAction<float>::getValueFromQVariant(QVariant v)
{
    return v.toFloat();
}

// just tell the compiler/linker that we exist
template class ArithmeticalAction<int>;
template class ArithmeticalAction<float>;
