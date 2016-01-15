#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_DIVIDE_ATTRIBUTE_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_DIVIDE_ATTRIBUTE_ACTION_H

#include "GameLogic/Actions/ArithmeticActionBase.h"
#include "GameLogic/Actions/Traits.h"

template <typename T>
class DivideAttributeAction : public ArithmeticActionBase<T>
{
public:
    typedef typename ArithmeticActionBase<T>::ExpressionType ExpressionType;

    DivideAttributeAction(GlobalState *state);
    DivideAttributeAction(GlobalState *state, const QDomElement &domElement);
    DivideAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr);

    virtual ~DivideAttributeAction() {}

    virtual void performAction() override;

    virtual QString type() override;
};


//------------------------------------------------------------------------------
template <typename T>
DivideAttributeAction<T>::DivideAttributeAction(GlobalState *state) :
        ArithmeticActionBase<T>(state)
{}

//------------------------------------------------------------------------------
template <typename T>
DivideAttributeAction<T>::DivideAttributeAction(GlobalState *state, const QDomElement &domElement) :
        ArithmeticActionBase<T>(state, domElement)
{}

//------------------------------------------------------------------------------
template <typename T>
DivideAttributeAction<T>::DivideAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr) :
        ArithmeticActionBase<T>(state, key, expr)
{}

//------------------------------------------------------------------------------
template <typename T>
void DivideAttributeAction<T>::performAction()
{
    const QVariant &variant = this->m_globalState->getValue(this->m_lhsKey);
    auto lhs = variant.value<T>();
    auto rhs = this->m_rhsExpr->evaluate();

    this->m_globalState->setValue(this->m_lhsKey, QVariant(lhs / rhs));
}

//------------------------------------------------------------------------------
template <typename T>
QString DivideAttributeAction<T>::type()
{
    return QString(traits::action_name<DivideAttributeAction<T>>::value);
}


namespace traits
{

    template <>
    struct action_name<DivideAttributeAction<int>>
    {
        static constexpr const char *value = "idiv";
    };

    template <>
    struct action_name<DivideAttributeAction<double>>
    {
        static constexpr const char *value = "fdiv";
    };

}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_DIVIDE_ATTRIBUTE_ACTION_H
