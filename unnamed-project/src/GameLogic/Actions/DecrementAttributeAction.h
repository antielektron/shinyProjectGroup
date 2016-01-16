#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_DECREMENT_ATTRIBUTE_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_DECREMENT_ATTRIBUTE_ACTION_H

#include "GameLogic/Actions/ArithmeticActionBase.h"
#include "GameLogic/Traits.h"

template <typename T>
class DecrementAttributeAction : public ArithmeticActionBase<T>
{
public:
    typedef typename ArithmeticActionBase<T>::ExpressionType ExpressionType;

    DecrementAttributeAction(GlobalState *state);
    DecrementAttributeAction(GlobalState *state, const QDomElement &domElement);
    DecrementAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr);

    virtual ~DecrementAttributeAction() {}

    virtual void performAction() override;

    virtual QString type() override;
};


//------------------------------------------------------------------------------
template <typename T>
DecrementAttributeAction<T>::DecrementAttributeAction(GlobalState *state) :
        ArithmeticActionBase<T>(state)
{}

//------------------------------------------------------------------------------
template <typename T>
DecrementAttributeAction<T>::DecrementAttributeAction(GlobalState *state, const QDomElement &domElement) :
        ArithmeticActionBase<T>(state, domElement)
{}

//------------------------------------------------------------------------------
template <typename T>
DecrementAttributeAction<T>::DecrementAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr) :
        ArithmeticActionBase<T>(state, key, expr)
{}

//------------------------------------------------------------------------------
template <typename T>
void DecrementAttributeAction<T>::performAction()
{
    const QVariant &variant = this->m_globalState->getValue(this->m_lhsKey);
    auto lhs = variant.value<T>();
    auto rhs = this->m_rhsExpr->evaluate();

    this->m_globalState->setValue(this->m_lhsKey, QVariant(lhs - rhs));
}

//------------------------------------------------------------------------------
template <typename T>
QString DecrementAttributeAction<T>::type()
{
    return QString(traits::action_name<DecrementAttributeAction<T>>::value());
}


namespace traits
{
    template <>
    struct action_name<DecrementAttributeAction<int>>
    {
        static const char *value() { return "idec"; }
    };

    template <>
    struct action_name<DecrementAttributeAction<double>>
    {
        static const char *value() { return "fdec"; }
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_DECREMENT_ATTRIBUTE_ACTION_H
