#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_BINARY_PRECONDITION_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_BINARY_PRECONDITION_WIDGET_H

#include <QMessageBox>

#include "SceneEditor/GameLogic/BinaryBaseWidget.h"
#include "GameLogic/GlobalState.h"
#include "GameLogic/Preconditions/BinaryPreconditionBase.h"
#include "GameLogic/Factories/ExpressionFactory.h"

template <typename T>
class BinaryPreconditionWidget : public BinaryBaseWidget
{
public:
    typedef BinaryPreconditionBase<T> PreconditionType;
    typedef typename PreconditionType::ExpressionType ExpressionType;

    BinaryPreconditionWidget(std::shared_ptr<SceneEditorGame> game, PreconditionType *condition, QWidget *parent = nullptr);
    virtual ~BinaryPreconditionWidget();

protected:
    virtual void onApplyClicked() override;

private:
    PreconditionType *m_precondition;
};


template <typename T>
BinaryPreconditionWidget<T>::BinaryPreconditionWidget(std::shared_ptr<SceneEditorGame> game, PreconditionType *condition, QWidget *parent) :
        BinaryBaseWidget(game, parent),
        m_precondition(condition)
{
    // Set UI values
    m_lhs->setText(m_precondition->getLhs()->string());
    m_type->setText(m_precondition->type());
    m_rhs->setText(m_precondition->getRhs()->string());
}

template <typename T>
BinaryPreconditionWidget<T>::~BinaryPreconditionWidget()
{}

template <typename T>
void BinaryPreconditionWidget<T>::onApplyClicked()
{
    try
    {
        std::unique_ptr<ExpressionType> lhsExpr = Factory::createExpressionFromString<typename ExpressionType::ValueType>(m_game->getGlobalState(), m_lhs->text());
        m_precondition->setLhs(std::move(lhsExpr));

        std::unique_ptr<ExpressionType> rhsExpr = Factory::createExpressionFromString<typename ExpressionType::ValueType>(m_game->getGlobalState(), m_rhs->text());
        m_precondition->setRhs(std::move(rhsExpr));

        m_game->notifyEventChanged();
    }
    catch (std::exception &e)
    {
        QMessageBox::warning(this, "Warning", e.what());
    }
}


#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_BINARY_PRECONDITION_WIDGET_H
