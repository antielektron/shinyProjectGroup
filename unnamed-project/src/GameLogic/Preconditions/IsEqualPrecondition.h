#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H

#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/GlobalState.h"

template <typename T>
class IsEqualPrecondition : public BinaryPreconditionBase<T>
{
public:
    IsEqualPrecondition(GlobalState *state, std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
            BinaryPreconditionBase<T>(state, std::move(exprA), std::move(exprB))
    {}

    virtual ~IsEqualPrecondition() {};

    virtual bool evaluateCondition() override
    {
        return m_exprA->evaluate() == m_exprB->evaluate();
    }

    virtual QString toQString() override
    {
        return m_exprA->toQString() + "=" + m_exprB->toQString();
    }

protected:
    using BinaryPreconditionBase<T>::m_exprA;
    using BinaryPreconditionBase<T>::m_exprB;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H
