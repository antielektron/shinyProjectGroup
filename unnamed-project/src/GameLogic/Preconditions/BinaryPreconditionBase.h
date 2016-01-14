#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H

#include <memory>

#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Preconditions/Expressions/ExpressionConstant.h"

template <typename T>
class BinaryPreconditionBase : public PreconditionBase
{
public:
    BinaryPreconditionBase() :
            m_exprA(new ExpressionConstant<T>()),
            m_exprB(new ExpressionConstant<T>())
    {}
    BinaryPreconditionBase(std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
            m_exprA(std::move(exprA)),
            m_exprB(std::move(exprB))
    {}

    virtual ~BinaryPreconditionBase() {}

    virtual QString string()
    {
        return m_exprA->string() + " " + this->name() + " " + m_exprB->string();
    }

protected:
    std::unique_ptr<Expression<T>> m_exprA;
    std::unique_ptr<Expression<T>> m_exprB;
};


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
