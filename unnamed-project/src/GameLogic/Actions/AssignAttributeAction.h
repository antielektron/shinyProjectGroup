#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ASSIGN_ATTRIBUTE_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ASSIGN_ATTRIBUTE_ACTION_H

#include "GameLogic/Actions/ArithmeticActionBase.h"
#include "GameLogic/Traits.h"

template <typename T>
class AssignAttributeAction : public ArithmeticActionBase<T>
{
public:
    typedef typename ArithmeticActionBase<T>::ExpressionType ExpressionType;

    AssignAttributeAction(GlobalState *state);
    AssignAttributeAction(GlobalState *state, const QDomElement &domElement);
    AssignAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr);

    virtual ~AssignAttributeAction() {}

    virtual void performAction() override;

    virtual QString type() override;
};


//------------------------------------------------------------------------------
template <typename T>
AssignAttributeAction<T>::AssignAttributeAction(GlobalState *state) :
        ArithmeticActionBase<T>(state)
{}

//------------------------------------------------------------------------------
template <typename T>
AssignAttributeAction<T>::AssignAttributeAction(GlobalState *state, const QDomElement &domElement) :
        ArithmeticActionBase<T>(state, domElement)
{}

//------------------------------------------------------------------------------
template <typename T>
AssignAttributeAction<T>::AssignAttributeAction(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr) :
        ArithmeticActionBase<T>(state, key, expr)
{}

//------------------------------------------------------------------------------
template <typename T>
void AssignAttributeAction<T>::performAction()
{
    QVariant value(this->m_rhsExpr->evaluate());
    this->m_globalState->setValue(this->m_lhsKey, value);
}

//------------------------------------------------------------------------------
template <typename T>
QString AssignAttributeAction<T>::type()
{
    return QString(traits::action_name<AssignAttributeAction<T>>::value);
}


namespace traits
{
    template <>
    struct action_name<AssignAttributeAction<int>>
    {
        static constexpr const char *value = "imov";
    };

    template <>
    struct action_name<AssignAttributeAction<double>>
    {
    static constexpr const char *value = "fmov";
    };

    template <>
    struct action_name<AssignAttributeAction<bool>>
    {
    static constexpr const char *value = "bmov";
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ASSIGN_ATTRIBUTE_ACTION_H
