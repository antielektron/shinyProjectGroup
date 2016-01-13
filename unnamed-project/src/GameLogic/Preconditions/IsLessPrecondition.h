#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_LESS_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_LESS_PRECONDITION_H

#include <QString>

#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/GlobalState.h"

template <typename T>
class IsLessPrecondition : public BinaryPreconditionBase
{
public:
    IsLessPrecondition(GlobalState *state,const QString &fieldA, const QString &fieldB);
    virtual ~IsLessPrecondition() {};

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;
};


//------------------------------------------------------------------------------
template <typename T>
IsLessPrecondition<T>::IsLessPrecondition(GlobalState *globalState, const QString &fieldA, const QString &fieldB) :
        BinaryPreconditionBase(globalState, fieldA, fieldB)
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template <typename T>
bool IsLessPrecondition<T>::evaluateCondition()
{
    const T &a = m_globalState->getValue(m_fieldA).value<T>();
    const T &b = m_globalState->getValue(m_fieldB).value<T>();

    return a < b;
}

//------------------------------------------------------------------------------
template <typename T>
QString IsLessPrecondition<T>::toQString()
{
    return m_fieldA + "<" + m_fieldA;
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_LESS_PRECONDITION_H
