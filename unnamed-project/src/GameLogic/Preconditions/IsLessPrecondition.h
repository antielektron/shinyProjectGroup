#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_LESS_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_LESS_PRECONDITION_H

#include <QString>

#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/GlobalState.h"

template <typename T>
class IsLessPrecondition : public BinaryPreconditionBase<T>
{
public:
    IsLessPrecondition(GlobalState *state, std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB);
    virtual ~IsLessPrecondition() {};

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:
    using BinaryPreconditionBase<T>::m_exprA;
    using BinaryPreconditionBase<T>::m_exprB;
};


//------------------------------------------------------------------------------
template <typename T>
IsLessPrecondition<T>::IsLessPrecondition(GlobalState *globalState, std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
        BinaryPreconditionBase<T>(globalState, std::move(exprA), std::move(exprB))
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template <typename T>
bool IsLessPrecondition<T>::evaluateCondition()
{
    return m_exprA->evaluate() < m_exprB->evaluate();
}

//------------------------------------------------------------------------------
template <typename T>
QString IsLessPrecondition<T>::toQString()
{
    return m_exprA->toQString() + "<" + m_exprB->toQString();
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_LESS_PRECONDITION_H
