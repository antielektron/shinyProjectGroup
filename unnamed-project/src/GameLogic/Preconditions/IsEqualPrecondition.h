#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H

#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/Preconditions/Traits.h"

template <typename T>
class IsEqualPrecondition : public BinaryPreconditionBase<T>
{
public:
    IsEqualPrecondition() = default;
    IsEqualPrecondition(std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
            BinaryPreconditionBase<T>(std::move(exprA), std::move(exprB))
    {}

    virtual ~IsEqualPrecondition() {};

    virtual bool evaluateCondition() override
    {
        return m_exprA->evaluate() == m_exprB->evaluate();
    }

    virtual QString name() override
    {
        return QString(traits::precondition_name<IsEqualPrecondition<T>>::value);
    }

protected:
    using BinaryPreconditionBase<T>::m_exprA;
    using BinaryPreconditionBase<T>::m_exprB;
};


namespace traits
{

    template <>
    struct precondition_name<IsEqualPrecondition<int>>
    {
        static constexpr const char *value = "ieq";
    };

    template <>
    struct precondition_name<IsEqualPrecondition<double>>
    {
        static constexpr const char *value = "feq";
    };

    template <>
    struct precondition_name<IsEqualPrecondition<bool>>
    {
        static constexpr const char *value = "beq";
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H
