#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_IS_EQUAL_PRECONDITION_H

#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/Preconditions/Traits.h"

template <typename T>
class IsEqualPrecondition : public BinaryPreconditionBase<T>
{
public:
    IsEqualPrecondition() = default;
    IsEqualPrecondition(const QDomElement &domElement);
    IsEqualPrecondition(std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB);
    virtual ~IsEqualPrecondition() {};

    virtual bool evaluateCondition() override;

    virtual QString type() override;

protected:
    using BinaryPreconditionBase<T>::m_exprA;
    using BinaryPreconditionBase<T>::m_exprB;
};


//------------------------------------------------------------------------------
template <typename T>
IsEqualPrecondition<T>::IsEqualPrecondition(const QDomElement &domElement) :
        BinaryPreconditionBase<T>(domElement)
{}

//------------------------------------------------------------------------------
template <typename T>
IsEqualPrecondition<T>::IsEqualPrecondition(std::unique_ptr<Expression<T>> exprA, std::unique_ptr<Expression<T>> exprB) :
        BinaryPreconditionBase<T>(std::move(exprA), std::move(exprB))
{}

//------------------------------------------------------------------------------
template <typename T>
bool IsEqualPrecondition<T>::evaluateCondition()
{
    return m_exprA->evaluate() == m_exprB->evaluate();
}

//------------------------------------------------------------------------------
template <typename T>
QString IsEqualPrecondition<T>::type()
{
    return QString(traits::precondition_name<IsEqualPrecondition<T>>::value);
}


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
