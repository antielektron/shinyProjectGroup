#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INCREMENT_ATTRIBUTE_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INCREMENT_ATTRIBUTE_ACTION_H

#include "GameLogic/Actions/ArithmeticActionBase.h"
#include "GameLogic/Actions/Traits.h"

template <typename T>
class IncrementAttributeAction : public ArithmeticActionBase<T>
{
public:
    typedef typename ArithmeticActionBase<T>::ExpressionType ExpressionType;

    IncrementAttributeAction(GlobalState *state);
    IncrementAttributeAction(GlobalState *state, const QDomElement &domElement);
    IncrementAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr);

    virtual ~IncrementAttributeAction() {}

    virtual void performAction() override;

    virtual QString type() override;
};


//------------------------------------------------------------------------------
template <typename T>
IncrementAttributeAction<T>::IncrementAttributeAction(GlobalState *state) :
        ArithmeticActionBase<T>(state)
{}

//------------------------------------------------------------------------------
template <typename T>
IncrementAttributeAction<T>::IncrementAttributeAction(GlobalState *state, const QDomElement &domElement) :
        ArithmeticActionBase<T>(state, domElement)
{}

//------------------------------------------------------------------------------
template <typename T>
IncrementAttributeAction<T>::IncrementAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr) :
        ArithmeticActionBase<T>(state, key, expr)
{}

//------------------------------------------------------------------------------
template <typename T>
void IncrementAttributeAction<T>::performAction()
{
    const QVariant &variant = this->m_globalState->getValue(this->m_lhsKey);
    auto lhs = variant.value<T>();
    auto rhs = this->m_rhsExpr->evaluate();

    this->m_globalState->setValue(this->m_lhsKey, QVariant(lhs + rhs));
}

//------------------------------------------------------------------------------
template <typename T>
QString IncrementAttributeAction<T>::type()
{
    return QString(traits::action_name<IncrementAttributeAction<T>>::value);
}


namespace traits
{

    template <>
    struct action_name<IncrementAttributeAction<int>>
    {
        static constexpr const char *value = "iadd";
    };

    template <>
    struct action_name<IncrementAttributeAction<double>>
    {
        static constexpr const char *value = "fadd";
    };

}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INCREMENT_ATTRIBUTE_ACTION_H
