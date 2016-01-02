#include "GameLogic/ArithmeticalAction.h"

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
    T valA = static_cast<T>(m_globalState->getValue(m_valA));
    T valB = static_cast<T>(m_globalState->getValue(m_valB));
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
