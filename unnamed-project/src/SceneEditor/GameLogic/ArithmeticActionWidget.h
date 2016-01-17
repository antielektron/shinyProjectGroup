#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_ARITHMETIC_ACTION_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_ARITHMETIC_ACTION_WIDGET_H

#include <QMessageBox>

#include "SceneEditor/GameLogic/BinaryBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"
#include "GameLogic/Actions/ArithmeticActionBase.h"
#include "GameLogic/Factories/ExpressionFactory.h"

template <typename T>
class ArithmeticActionWidget : public BinaryBaseWidget
{
public:
    typedef ArithmeticActionBase<T> ActionType;
    typedef typename ActionType::ExpressionType ExpressionType;

    ArithmeticActionWidget(std::shared_ptr<SceneEditorGame> game, ActionType *action, QWidget *parent = nullptr);
    virtual ~ArithmeticActionWidget();

protected:
    virtual void onApplyClicked() override;

private:
    ActionType *m_action;
};


template <typename T>
ArithmeticActionWidget<T>::ArithmeticActionWidget(std::shared_ptr<SceneEditorGame> game, ActionType *action, QWidget *parent) :
        BinaryBaseWidget(game, parent),
        m_action(action)
{
    // Set UI values
    m_lhs->setText(m_action->getLhs());
    m_type->setText(m_action->type());
    m_rhs->setText(m_action->getRhs()->string());
}

template <typename T>
ArithmeticActionWidget<T>::~ArithmeticActionWidget()
{}

template <typename T>
void ArithmeticActionWidget<T>::onApplyClicked()
{
    try
    {
        m_action->setLhs(m_lhs->text());

        std::unique_ptr<ExpressionType> rhsExpr = Factory::createExpressionFromString<typename ExpressionType::ValueType>(m_game->getGlobalState(), m_rhs->text());
        m_action->setRhs(std::move(rhsExpr));

        m_game->notifyEventChanged();
    }
    catch (std::exception &e)
    {
        QMessageBox::warning(this, "Warning", e.what());
    }
}


#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_ARITHMETIC_ACTION_WIDGET_H
