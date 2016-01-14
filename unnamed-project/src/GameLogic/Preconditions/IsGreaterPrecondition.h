#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_GREATER_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_GREATER_PRECONDITION_H

#include <QString>

#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/GlobalState.h"
#include "GameLogic/Preconditions/Traits.h"

template <typename T>
class IsGreaterPrecondition : public BinaryPreconditionBase<T>
{
public:
    IsGreaterPrecondition(GlobalState *state, std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB);
    virtual ~IsGreaterPrecondition() {};

    virtual bool evaluateCondition() override;

    virtual QString name() override
    {
        return QString(traits::precondition_name<IsGreaterPrecondition<T>>::value);
    }

protected:
    using BinaryPreconditionBase<T>::m_exprA;
    using BinaryPreconditionBase<T>::m_exprB;
};


//------------------------------------------------------------------------------
template <typename T>
IsGreaterPrecondition<T>::IsGreaterPrecondition(GlobalState *globalState, std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
        BinaryPreconditionBase<T>(globalState, std::move(exprA), std::move(exprB))
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
template <typename T>
bool IsGreaterPrecondition<T>::evaluateCondition()
{
    return m_exprA->evaluate() > m_exprB->evaluate();
}


namespace traits
{

    template <>
    struct precondition_name<IsGreaterPrecondition<int>>
    {
        static constexpr const char *value = "igreater";
    };

    template <>
    struct precondition_name<IsGreaterPrecondition<double>>
    {
        static constexpr const char *value = "fgreater";
    };

}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_GREATER_PRECONDITION_H
