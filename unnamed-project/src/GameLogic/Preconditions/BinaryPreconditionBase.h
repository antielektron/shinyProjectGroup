#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H

#include <memory>
#include <QString>

#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Preconditions/Expressions/Expression.h"

template <typename T>
class BinaryPreconditionBase : public PreconditionBase
{
public:
    BinaryPreconditionBase(GlobalState *globalState, std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
            PreconditionBase(globalState),
            m_exprA(std::move(exprA)),
            m_exprB(std::move(exprB))
    {}

    virtual ~BinaryPreconditionBase() {}

    virtual QString string()
    {
        m_exprA->toQString() + " " + this->name() + " " + m_exprB->toQString();
    }

protected:
    std::unique_ptr<Expression<T>> m_exprA;
    std::unique_ptr<Expression<T>> m_exprB;
};


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
