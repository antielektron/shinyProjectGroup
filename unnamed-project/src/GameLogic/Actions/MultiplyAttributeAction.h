#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_MULTIPLY_ATTRIBUTE_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_MULTIPLY_ATTRIBUTE_ACTION_H

#include "GameLogic/Actions/ArithmeticActionBase.h"
#include "GameLogic/Traits.h"

template <typename T>
class MultiplyAttributeAction : public ArithmeticActionBase<T>
{
public:
    typedef typename ArithmeticActionBase<T>::ExpressionType ExpressionType;

    MultiplyAttributeAction(GlobalState *state);
    MultiplyAttributeAction(GlobalState *state, const QDomElement &domElement);
    MultiplyAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr);

    virtual ~MultiplyAttributeAction() {}

    virtual void performAction() override;

    virtual QString type() override;
};


//------------------------------------------------------------------------------
template <typename T>
MultiplyAttributeAction<T>::MultiplyAttributeAction(GlobalState *state) :
        ArithmeticActionBase<T>(state)
{}

//------------------------------------------------------------------------------
template <typename T>
MultiplyAttributeAction<T>::MultiplyAttributeAction(GlobalState *state, const QDomElement &domElement) :
        ArithmeticActionBase<T>(state, domElement)
{}

//------------------------------------------------------------------------------
template <typename T>
MultiplyAttributeAction<T>::MultiplyAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr) :
        ArithmeticActionBase<T>(state, key, expr)
{}

//------------------------------------------------------------------------------
template <typename T>
void MultiplyAttributeAction<T>::performAction()
{
    const QVariant &variant = this->m_globalState->getValue(this->m_lhsKey);
    auto lhs = variant.value<T>();
    auto rhs = this->m_rhsExpr->evaluate();

    this->m_globalState->setValue(this->m_lhsKey, QVariant(lhs * rhs));
}

//------------------------------------------------------------------------------
template <typename T>
QString MultiplyAttributeAction<T>::type()
{
    return QString(traits::action_name<MultiplyAttributeAction<T>>::value());
}


namespace traits
{
    template <>
    struct action_name<MultiplyAttributeAction<int>>
    {
        static const char *value() { return "imul"; }
    };

    template <>
    struct action_name<MultiplyAttributeAction<double>>
    {
        static const char *value() { return "fmul"; }
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_MULTIPLY_ATTRIBUTE_ACTION_H
