#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H

#include <QString>

#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/GlobalState.h"

template <typename T>
class IsEqualPrecondition : public BinaryPreconditionBase
{
public:
    IsEqualPrecondition(GlobalState *state,const QString &objA, const QString &objB);
    virtual ~IsEqualPrecondition() {};

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;
};


//------------------------------------------------------------------------------
template <typename T>
IsEqualPrecondition<T>::IsEqualPrecondition(GlobalState *globalState, const QString &fieldA, const QString &fieldB) :
        BinaryPreconditionBase(globalState, fieldA, fieldB)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template <typename T>
bool IsEqualPrecondition<T>::evaluateCondition()
{
    const T &a = m_globalState->getValue(m_fieldA).value<T>();
    const T &b = m_globalState->getValue(m_fieldB).value<T>();

    return a == b;
}

//------------------------------------------------------------------------------
template <typename T>
QString IsEqualPrecondition<T>::toQString()
{
    return m_fieldA + "=" + m_fieldA;
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H
